#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include "winsock2.h"
#include "ws2tcpip.h"

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> /* close */
#include <machine/byte_order.h>

#endif

#include "MulticastCommon.h"


class MulticastServer {
public:
    MulticastServer();
    ~MulticastServer();
    
    int open( char * address, int port );
    int send( void );
    void MulticastServer::beginBuffer( void );
    void MulticastServer::packBuffer( char value );
    void MulticastServer::packBuffer( short value );
    void MulticastServer::packBuffer( int value );
    void MulticastServer::packBuffer( float value );
    void MulticastServer::packBuffer( double value );
    void MulticastServer::packBuffer( char * str );
    void MulticastServer::setBufferSize( int bsize );
    void MulticastServer::close( void );
    
protected:
    int sd;
    struct sockaddr_in clientAddr;
    struct sockaddr_in serverAddr;
    int bufferSize;
    unsigned char * buffer;
    unsigned char * p;
};