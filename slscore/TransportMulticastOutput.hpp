#ifndef _TransportMulticastOutput_INCLUDE_
#define _TransportMulticastOutput_INCLUDE_

#include <libltntstools/ltntstools.h>

class CTransportMulticastOutput
{
public :
	CTransportMulticastOutput(const char *ip, int port);
	~CTransportMulticastOutput();

	int write(unsigned char *pkts, int packetCount);

private:
	unsigned char *sendBuffer;
	int sendIndex;
	int minSendBytes;
	int skt;
	struct sockaddr_in sin;
};

#endif
