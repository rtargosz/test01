// Server.cpp
#include "server.h"

// Perform basic initialization not appropriate for constructor
int TLVServer::Init()
{
    // Set default port value
	m_Port = DEFAULT_PORT;
    
    // Allocate memory for temporary buffers (initialized later)
    try
    {
        for (auto i = 0; i < MAX_CLIENTS; i++)
            m_ResumeBuffer[i] = m_Allocator.allocate(MAX_RESUME_BYTES * sizeof(uint8_t));
        
        m_TempTlvData = m_Allocator.allocate((MAX_BYTES + MAX_RESUME_BYTES) * sizeof(uint8_t));
    }
    catch (const std::bad_alloc& e)
    {
        // Memory allocation failure
        cout << "Memory allocation failure: " << errno << endl;
        exit(EXIT_FAILURE);
    }

	return 0;
}

// Start server (create socket, listener and bind)
int TLVServer::Startup(void)
{
	// Create a master Internet Streaming socket to listen for connections
	if ((m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
        // File descriptor is invalid
		cout << "socket failed: " << errno << endl;
		exit(EXIT_FAILURE);
	}

	// Set master socket to allow multiple connections and reuse the same address
	auto options = 0;
	if (setsockopt(m_ServerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&options, sizeof(options)) < 0)
	{
        // Setting socket options failed
		cout << "setsockopt call failed: " << errno << endl;
		exit(EXIT_FAILURE);
	}

	// Type/location of socket to create
	struct sockaddr_in srvAddress;
	srvAddress.sin_addr.s_addr = INADDR_ANY;
	srvAddress.sin_family = AF_INET;
	srvAddress.sin_port = htons(m_Port);

	// Bind the socket to the new address structure
	if (bind(m_ServerSocket, (struct sockaddr *)&srvAddress, sizeof(srvAddress)) < 0)
	{
        // Bind operation failed
		cout << "bind failed: " << errno << endl;
		exit(EXIT_FAILURE);
	}

	// Try to specify maximum of 3 pending connections for the master socket
	if (listen(m_ServerSocket, 3) < 0)
	{
        // Listen operation failed
		cout << "listen call failed: " << errno << endl;
		exit(EXIT_FAILURE);
	}

	// Ready to accept incoming connections
	cout << "TLV parsing server waiting for connections on port " << m_Port << "..." << endl;

	return 0;
}

int TLVServer::HandleConnections()
{
    struct sockaddr_in clientAddress;
    int clientAddressLength;
	fd_set ReadFds;
    
	for (;;)
	{
#ifdef _SELECT    
		// Clear the socket set
		FD_ZERO(&ReadFds);

		// Add master socket to set
		FD_SET(m_ServerSocket, &ReadFds);
		auto MaxSD = m_ServerSocket;

		// Add child sockets to set
		for (auto i = 0 ; i < MAX_CLIENTS ; i++)
		{
			// If valid socket descriptor then add to read list
			if (m_ClientSockets[i] > 0)
				FD_SET(m_ClientSockets[i], &ReadFds);

			// Highest file descriptor number, need it for the select function
			if (m_ClientSockets[i] > MaxSD)
				MaxSD = m_ClientSockets[i];
		}

		// Wait for activity on one of the sockets
		auto socketActivity = select(MaxSD + 1, &ReadFds, NULL, NULL, NULL);

		if ((socketActivity < 0) && (errno != EINTR))
		{
			cout << "select error: " << errno << endl;
		}
		
		// If something happened on the master socket then its an incoming connection
		if (FD_ISSET(m_ServerSocket, &ReadFds))
		{
			auto ConnectionSocket = 0;
			if ((ConnectionSocket = accept(m_ServerSocket, (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddressLength))<0)
			{
				cout << "accept failure: " << errno << endl;
				exit(EXIT_FAILURE);
			}

			// Add new socket to array of sockets
			for (auto i = 0; i < MAX_CLIENTS; i++)
			{
				// If position is empty
				if (m_ClientSockets[i] == 0)
				{
					m_ClientSockets[i] = ConnectionSocket;
					break; // exit loop
				}
			}
		}

		// Handle IO operations on all sockets
		for (auto i = 0; i < MAX_CLIENTS; i++)
		{
			if (FD_ISSET(m_ClientSockets[i], &ReadFds))
			{
				// Check if it was for closing, and also read the incoming message
                uint8_t buffer[MAX_BYTES + 1];
				auto ValueRead = read(m_ClientSockets[i], buffer, MAX_BYTES);
				if (ValueRead == 0)
				{
					// Somebody disconnected, get his details and print
					getpeername(m_ClientSockets[i], (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddressLength);

					// Close the socket and mark as 0 in list for reuse
					if (close(m_ClientSockets[i]) < 0)
                        cout << "close failure: " << errno << endl;
                    
					m_ClientSockets[i] = 0;
				}
				else if (ValueRead > 0)
				{
                    // Convert client address and port to a string for display purposes
                    char *tmpStr = inet_ntoa(clientAddress.sin_addr);
                    strcat(tmpStr, ":");
                    auto tempStr2 = to_string(ntohs(clientAddress.sin_port));
                    strcat(tmpStr, tempStr2.c_str());
                    PrintTLVs(buffer, ValueRead, tmpStr, i);
				}
			}
		}
#else // use epoll
        struct epoll_event m_Events[MAX_CLIENTS];
        m_ServerSocket = epoll_create1(MAX_CLIENTS);
    
		// Add child sockets to set
		for (auto i = 0 ; i < MAX_CLIENTS ; i++)
		{
            m_Events[i].events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
            m_Events[i].data.fd = m_ClientSockets[i];
            auto res = epoll_ctl(m_ServerSocket, EPOLL_CTL_ADD, m_ClientSockets[i], &m_Events[i]);
		}    
        auto nfds = epoll_wait(m_ServerSocket, m_Events, MAX_CLIENTS * 4, -1);
        if (nfds < 0)
            cout << "Error in epoll_wait!" << endl;
  
        // for each ready socket
        for(auto i = 0; i < MAX_CLIENTS; i++)
        {
            auto fd = m_Events[i].data.fd;
            //handle_io_on_socket(fd);
        }
#endif
	}

	return 0;
}

void TLVServer::Shutdown()
{
    cout << "Stopping TLV parsing server..." << endl;
}

// Check command line arguments for port number
int TLVServer::CheckOptions(int argc, char **argv)
{
	for (auto i = 1; i < argc; i++)
	{
		if ((argv[i][0] == '-') || (argv[i][0] == '/'))
		{
			switch (tolower(argv[i][1]))
			{
			case 'p':
				if (strlen(argv[i]) > 3)
				{
					auto temp = &argv[i][3];
					m_Port = stoi(temp);
					if (m_Port > 999 && m_Port < 65536)
					{
                        // Good port, return OK
						return 0;
					}
				}

				cout << "port must be between 1000 and 65536" << endl;
                exit(EXIT_FAILURE);
				break;
			default:
				break;
			}
		}
	}
	
    cout << "Usage: tlvdump [-p:port]" << endl;
    cout << "  -p:port listener port number" << endl;
    exit(EXIT_FAILURE);
}

// Parse and print TLVs, resuming across packets PER CONNECTION
int TLVServer::PrintTLVs(uint8_t* tlvData, int length, char* sourceString, int clientFD)
{
    // Not enough data in buffer for even a single element
    if (length < 6) return -1;
    
    // Keep track of variable offset to next unread position
    uint32_t tlvOffset = 0;
    // Storage for calculated length of data
    uint32_t dataLen = 0;
    
    // Initialize temporary buffer
    m_Allocator.construct(m_TempTlvData);
    
    // Check whether a resume is pending for this connection on any of the elements
    if (m_ResumePrint[clientFD] != 0)
    {
        // Pre-pend buffer bytes to current bytes
        memcpy(m_TempTlvData, m_ResumeBuffer[clientFD], MAX_RESUME_BYTES * sizeof(uint8_t));
        // Destroy old resume buffer
        m_Allocator.destroy(m_ResumeBuffer[clientFD]);
        // Clear resume flag
        m_ResumePrint[clientFD] = 0;
    }

    // Append new data to temp buffer
    memcpy(m_TempTlvData, tlvData, MAX_BYTES * sizeof(uint8_t));
    
	// Parse the data for TLV information
    for(auto i = 0 ; i < length; i += tlvOffset)
    {
		// Determine current type
        if (m_TempTlvData[i+0] == 0xE1 && m_TempTlvData[i+1] == 0x10)
        {
			// Reached end of data, setup to resume on next packet PER CONNECTION
			if (length < i+TLV_HEADER_SIZE)
            {
                // Set to resume hello element
                m_ResumePrint[clientFD] = 1;
                // Save next MAX_RESUME_BYTES bytes of data to resume buffer
                m_Allocator.construct(m_ResumeBuffer[clientFD]);
                memcpy(m_ResumeBuffer[clientFD], &m_TempTlvData[i], MAX_RESUME_BYTES * sizeof(uint8_t));
                // Clean up the TLV buffer and return
                m_Allocator.destroy(m_TempTlvData);
                return 0;
            }
            // Parse the data length together into a 32-bit integer
            dataLen = (uint32_t)(((uint8_t)m_TempTlvData[i+2] << 24) | ((uint8_t)m_TempTlvData[i+3] << 16) | ((uint8_t)m_TempTlvData[i+4] << 8) | (uint8_t)m_TempTlvData[i+5]);
			cout << "[" << sourceString <<"] [";
			cout << "Hello] [" << dataLen << "] []" << endl;
            // No data should be in hello element
            if (dataLen != 0) return -1;
            tlvOffset = TLV_HEADER_SIZE;
        }
		else if (m_TempTlvData[i+0] == 0x0B && m_TempTlvData[i+1] == 0x1E)
        {
			// Reached end of data, setup to resume on next packet PER CONNECTION
			if (length < i+TLV_HEADER_SIZE)
            {
                // Set to resume goodbye element
                m_ResumePrint[clientFD] = 2;
                // Save next MAX_RESUME_BYTES bytes of data to resume buffer
                m_Allocator.construct(m_ResumeBuffer[clientFD]);
                memcpy(m_ResumeBuffer[clientFD], &m_TempTlvData[i], MAX_RESUME_BYTES * sizeof(uint8_t));
                // Clean up the TLV buffer and return
                m_Allocator.destroy(m_TempTlvData);
                return 0;
            }
            // Parse the data length together into a 32-bit integer
            dataLen = (uint32_t)(((uint8_t)m_TempTlvData[i+2] << 24) | ((uint8_t)m_TempTlvData[i+3] << 16) | ((uint8_t)m_TempTlvData[i+4] << 8) | (uint8_t)m_TempTlvData[i+5]);
			cout << "[" << sourceString <<"] [";
			cout << "Goodbye] [" << dataLen << "] []" << endl;
            
            // No data should be in goodbye element
            if (dataLen != 0) return -1;
            
            tlvOffset = TLV_HEADER_SIZE;
        }
		else if (m_TempTlvData[i+0] == 0xDA && m_TempTlvData[i+1] == 0x7A)
        {
			// Reached end of data, setup to resume on next packet PER CONNECTION
			if (length < i+TLV_HEADER_SIZE+4) // 4 extra bytes required for data
            {
                // Set to resume data element
                m_ResumePrint[clientFD] = 3;
                // Save next MAX_RESUME_BYTES bytes of data to resume buffer
                m_Allocator.construct(m_ResumeBuffer[clientFD]);
                memcpy(m_ResumeBuffer[clientFD], &m_TempTlvData[i], MAX_RESUME_BYTES * sizeof(uint8_t));
                // Clean up the TLV buffer and return
                m_Allocator.destroy(m_TempTlvData);
                return 0;
            }
            // Parse the data length together into a 32-bit integer
            dataLen = (uint32_t)(((uint8_t)m_TempTlvData[i+2] << 24) | ((uint8_t)m_TempTlvData[i+3] << 16) | ((uint8_t)m_TempTlvData[i+4] << 8) | (uint8_t)m_TempTlvData[i+5]);
			 // Data - display length first four bytes
			cout << "[" << sourceString <<"] [";
			cout << "Data] [" << dataLen << "] [";
            
            // Display first 4 bytes of data
			cout << Hex2String(m_TempTlvData[i+6]);
			cout << " " << Hex2String(m_TempTlvData[i+7]);
			cout << " " << Hex2String(m_TempTlvData[i+8]);
			cout << " " << Hex2String(m_TempTlvData[i+9]);
            cout << "]" << endl;
            
            tlvOffset = TLV_HEADER_SIZE + dataLen;
        }
		else
        {
            // Check next position for valid element
            tlvOffset = 1;
        }
    }
    
    return 0;
}

string TLVServer::Hex2String(uint8_t data)
{
    string s(4, ' ');
    s[0] = '0';
    s[1] = 'x';
    s[2] = hexmap[(data & 0xF0) >> 4];
    s[3] = hexmap[data & 0x0F];
    return s;
}
