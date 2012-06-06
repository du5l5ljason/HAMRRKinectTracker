/*
 *  MulticastClient.h
 *  Multiclient
 *
 *  Created by Loren Olson on 11/11/05.
 *
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "MulticastCommon.h"


class MulticastClient {
public:
    MulticastClient();
    ~MulticastClient();
    
    int open( char * address, int port );
    int read( void );
    void MulticastClient::beginUnpack( void );
    char MulticastClient::unpackChar( void );
    short MulticastClient::unpackShort( void );
    int MulticastClient::unpackInt( void );
    float MulticastClient::unpackFloat( void );
    double MulticastClient::unpackDouble( void );
    char * MulticastClient::unpackString( void );
    void MulticastClient::setBufferSize( int bsize );
    void MulticastClient::close( void );
    
protected:
    int sd;
    struct sockaddr_in clientAddr;
    struct sockaddr_in serverAddr;
    int bufferSize;
    unsigned char * buffer;
    unsigned char * p;
};