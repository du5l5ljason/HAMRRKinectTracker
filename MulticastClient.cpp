/*
 *  MulticastClient.cpp
 *  Multiclient
 *
 *  Created by Loren Olson on 11/11/05.
 *
 */
#include "StdAfx.h"
#include "MulticastClient.h"


// constructor
MulticastClient::MulticastClient()
{
    sd = 0;
    bufferSize = 40000;
    buffer = (unsigned char *)malloc( bufferSize );
    if (buffer == NULL) perror( "MulticastClient::MulticastClient(), malloc" );
    p = buffer;
}


// destructor
MulticastClient::~MulticastClient()
{
    if (buffer) free( (void *)buffer );
    close();
}


int MulticastClient::open( char * address, int port )
{
    int rc;
	struct ip_mreq mreq;
	struct in_addr mcastAddr;
	struct hostent *h;

	WORD wVersionRequested;
	WSADATA wsaData;
    
	wVersionRequested = MAKEWORD( 2, 0 ); 
	if(WSAStartup( wVersionRequested, &wsaData ) != 0) {
		printf( "Multicast::open(), Windows Socket Initialization Error\n" );
		return 0;
	}
    
	h = gethostbyname( address );
	if (h == NULL) {
		printf( "Multicast::open(), gethostbyname failed '%s'\n", address );
		return 0;
	}
    
    memcpy(&mcastAddr, h->h_addr_list[0],h->h_length);
    
	/* check if dest address is multicast */
	if(!IN_MULTICAST(ntohl(mcastAddr.s_addr))) {
		printf( "Multicast::open(), address '%s' is not multicast \n", inet_ntoa(mcastAddr) );
		return 0;
	}
    
	/* create socket */
	sd = (int)socket( AF_INET, SOCK_DGRAM, 0 );
	if (sd < 0) {
		perror( "Multicast::open(), socket" );
		return 0;
	}
    
	/* bind any port number */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	serverAddr.sin_port = htons( port );
	if ( bind( sd, (struct sockaddr *) &serverAddr, sizeof(serverAddr) ) < 0 ) {
		perror( "Multicast::open(), bind" );
		return 0;
	}
    
    /* join multicast group */
	mreq.imr_multiaddr.s_addr=mcastAddr.s_addr;
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    
    rc = setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq));
	if (rc < 0) {
        perror( "Multicast::open(), setsockopt, IP_ADD_MEMBERSHIP" );
		return 0;
	}
    
    printf( "INFO: listening to mgroup %s:%d\n", inet_ntoa(mcastAddr), port);
    return 1;
}


int MulticastClient::read( void )
{
	memset(buffer, 0, bufferSize);
    socklen_t clientLen = sizeof( clientAddr );
    int n = recvfrom( sd, (char *)buffer, bufferSize, 0, (struct sockaddr *) &clientAddr, &clientLen );
    if( n < 0) {
        perror( "MulticastClient::read, recvfrom" );
    }
    return n;
}


void MulticastClient::beginUnpack( void )
{
    p = buffer;
}


char MulticastClient::unpackChar( void )
{
    char * cp = (char *)p;
	p += sizeof( char );
    return *cp;
}


short MulticastClient::unpackShort( void )
{
    short * sp = (short *)p;
	short value = SWAP_SHORT( *sp );
	p += sizeof( short );
    return value;
}


int MulticastClient::unpackInt( void )
{
    int * ip = (int *)p;
	int value = SWAP_LONG( *ip );
	p += sizeof( int );
    return value;
}


float MulticastClient::unpackFloat( void )
{
    float * fp = (float *)p;
	float value = SWAP_FLOAT( *fp );
	p += sizeof( float );
    return value;
}


double MulticastClient::unpackDouble( void )
{
    double * dp = (double *)p;
	double value = SWAP_DOUBLE( *dp );
	p += sizeof( double );
    return value;
}

// the application programmer using this class must take
// responsibility for doing a free on this stringBuffer that
// is returned.
char * MulticastClient::unpackString( void )
{
    char * q = (char *)p;
    int len = 0;
    while (*q != 0) {
        q++;
        len++;
    }
    char * stringBuffer = (char *)malloc( len + 1 );
    if (stringBuffer == NULL) {
        perror( "MulticastClient::unpackString(), malloc" );
        return NULL;
    }
    strcpy( stringBuffer, (char *)p );
    p += len + 1;
    return stringBuffer;
}


void MulticastClient::setBufferSize( int bsize )
{
    bufferSize = bsize;
    if (buffer) free( (void *)buffer );
    buffer = (unsigned char *)malloc( bufferSize );
    if (buffer == NULL) perror( "MulticastClient::setBufferSize(), malloc" );
    p = buffer;
}


void MulticastClient::close( void )
{

	closesocket( sd );
	WSACleanup();
    
}