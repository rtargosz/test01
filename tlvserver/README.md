This is a simple Linux select/accept server that listens on a specified port (-p:<port> at the command prompt) for connections,
then parses the incoming data for a TLV-style input as follows:
TYPE (2 bytes, network byte order), valid values:
- 0xE110 - Hello
- 0xDA7A - Data
- 0x0B1E - Goodbye
LENGTH (4 bytes, network byte order)
VALUE (variable length, number of bytes denoted by the “LENGTH” field)

It can resume parsing data on a connection across 1024-byte packet boundaries.
