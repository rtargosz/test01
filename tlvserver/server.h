// server.h

#pragma once

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <memory>
#include <iostream>

// Hex character mappings
constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Use the std namespace to keep code cleaner
using namespace std;

#define DEFAULT_PORT 8888    // Default port for server listener
#define MAX_CLIENTS 16       // Maximum allowed client connections
#define MAX_BYTES 1024       // Maximum bytes to read from client
#define MAX_RESUME_BYTES 16  // Maximum bytes to buffer between reads
#define TLV_HEADER_SIZE 6    // TLV header size = type + length

// Class definition for TLV server
// Normally, the server and data would be separated
// But since we're not doing much with the data
// It is just being keep to a single class
class TLVServer
{
public:
	TLVServer()	{ }

	// Perform basic initialization not appropriate for constructor
	int Init();
    // Check command line argument validity and get port number
	int CheckOptions(int argc, char **argv);
    // Start server socket listener and bind
	int Startup();
    // Wait for incoming connections (select/accept) and send to parser
	int HandleConnections();
    // Registered with atexit()
	static void Shutdown();

private:
    // Simple Hex value to string converter for printing
	string Hex2String(uint8_t data);
    
    // Port number for server listener (parsed from command line)
	int m_Port;
    // Socket ID for server listener
	int m_ServerSocket;
    // Socket IDs for client connections
	int *m_ClientSockets = new int[MAX_CLIENTS]();
    // Function to parse and print TLV data
	int PrintTLVs(uint8_t* buffer, int length, char* address, int client);
    
    // Keep track of whether a client parse/print needs to resume from next packet
	int *m_ResumePrint = new int[MAX_CLIENTS]();
    // Location to buffer data PER CONNECTION to resume on next packet
	//uint8_t m_ResumeBuffer[MAX_CLIENTS][MAX_RESUME_BYTES];
	uint8_t **m_ResumeBuffer = new uint8_t*[MAX_CLIENTS];
    // Temporary storage for TLV data buffer(s)
    uint8_t *m_TempTlvData;
    // Standard memory allocator
    allocator<uint8_t> m_Allocator;
    
};
