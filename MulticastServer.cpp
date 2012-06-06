#include "StdAfx.h"
#include "MulticastServer.h"

// constructor
MulticastServer::MulticastServer()
{
    sd = 0;
    bufferSize = 20000;
    buffer = (unsigned char *)malloc( bufferSize );
    if (buffer == NULL) perror( "MulticastServer::MulticastServer(), malloc" );
    p = buffer;
}


// destructor
MulticastServer::~MulticastServer()
{
    if (buffer) free( (void *)buffer );
    close();
}


int MulticastServer::open( char * address, int port )
{
    char ttl = 1;
	struct hostent *h;
//#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
    
	wVersionRequested = MAKEWORD( 2, 0 ); 
	if(WSAStartup( wVersionRequested, &wsaData ) != 0) {
		printf( "Multicast::open(), Windows Socket Initialization Error\n" );
		return 0;
	}
//#endif
    
	h = gethostbyname( address );
	if (h == NULL) {
		printf( "Multicast::open(), gethostbyname failed '%s'\n", address );
		return 0;
	}
    
	serverAddr.sin_family = h->h_addrtype;
	memcpy((char *) &serverAddr.sin_addr.s_addr, h->h_addr_list[0],h->h_length);
	serverAddr.sin_port = htons( port );
    
	/* check if dest address is multicast */
	if (!IN_MULTICAST( ntohl( serverAddr.sin_addr.s_addr ) )) {
		printf( "Multicast::open(), address '%s' is not multicast \n", inet_ntoa( serverAddr.sin_addr ) );
		return 0;
	}
    
	/* create socket */
	sd = (int)socket( AF_INET, SOCK_DGRAM, 0 );
	if (sd < 0) {
		perror( "Multicast::open(), socket" );
		return 0;
	}
    
	/* bind any port number */
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	clientAddr.sin_port = htons(0);
	if ( bind( sd, (struct sockaddr *) &clientAddr, sizeof(clientAddr) ) < 0 ) {
		perror( "Multicast::open(), bind" );
		return 0;
	}
    
	if (setsockopt( sd, IPPROTO_IP, IP_MULTICAST_TTL,  &ttl,sizeof(ttl) ) < 0) {
		perror( "Multicast::open(), setsockopt, TTL" );
		return 0;
	}
    
	printf("INFO: ready to send data on multicast group '%s' (%s)\n",
           h->h_name, inet_ntoa( *(struct in_addr *) h->h_addr_list[0]) );
    
    return 1;
}


int MulticastServer::send( void )
{
    int numSent = sendto( sd, (char *)buffer, (int)(p - buffer), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr) );
    if (numSent == -1) {
		perror( "MulticastServer::send, sendto" );
	}
    return numSent;
}


void MulticastServer::beginBuffer( void )
{
    p = buffer;
}


void MulticastServer::packBuffer( char value )
{
    char * cp = (char *)p;
	*cp = value;
	p += sizeof( char );
}


void MulticastServer::packBuffer( short value )
{
    short * sp = (short *)p;
	*sp = SWAP_SHORT( value );
	p += sizeof( short );
}


void MulticastServer::packBuffer( int value )
{
    int * ip = (int *)p;
	*ip = SWAP_LONG( value );
	p += sizeof( int );
}


void MulticastServer::packBuffer( float value )
{
    float * fp = (float *)p;
	*fp = SWAP_FLOAT( value );
	p += sizeof( float );
}


void MulticastServer::packBuffer( double value )
{
    double * dp = (double *)p;
	*dp = SWAP_DOUBLE( value );
	p += sizeof( double );
}


void MulticastServer::packBuffer( char * str )
{
    char * sp = (char *)p;
    int len = (int)strlen( str );
    strcpy( sp, str );
    p += len + 1;
}


void MulticastServer::setBufferSize( int bsize )
{
    bufferSize = bsize;
    if (buffer) free( (void *)buffer );
    buffer = (unsigned char *)malloc( bufferSize );
    if (buffer == NULL) perror( "MulticastServer::setBufferSize(), malloc" );
    p = buffer;
}


void MulticastServer::close( void )
{
#ifdef WIN32
	closesocket( sd );
	WSACleanup();
#endif    
}




