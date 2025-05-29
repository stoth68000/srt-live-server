#include <errno.h>
#include <string.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

#include "TransportMulticastOutput.hpp"

CTransportMulticastOutput::CTransportMulticastOutput(const char *ip, int port)
{
        minSendBytes = 1316;
        sendIndex = 0;
        skt = -1;
        sendBuffer = (unsigned char *)malloc(minSendBytes);

	skt = socket(AF_INET, SOCK_DGRAM, 0);

	sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = inet_addr(ip);

	/* Non-blocking required */
        int fl = fcntl(skt, F_GETFL, 0);
        if (fcntl(skt, F_SETFL, fl | O_NONBLOCK) < 0) {
        }


}

CTransportMulticastOutput::~CTransportMulticastOutput()
{
}

int CTransportMulticastOutput::write(unsigned char *pkts, int packetCount)
{
	int byteCount = packetCount * 188;

        ssize_t v = 0;
        if (minSendBytes == 0) {
                v = sendto(skt, pkts, byteCount, 0, (struct sockaddr *)&sin, sizeof(sin));
        } else {
                int len = byteCount;
                int offset = 0;
                int cplen;
                while (len > 0) {
                        if (len > (minSendBytes - sendIndex)) {
                                cplen = minSendBytes - sendIndex;
                        } else {
                                cplen = len;
                        }
                        memcpy(sendBuffer + sendIndex, pkts + offset, cplen);
                        sendIndex += cplen;
                        offset += cplen;
                        len -= cplen;

                        if (sendIndex == minSendBytes) {
                                v += sendto(skt, sendBuffer, minSendBytes, 0, (struct sockaddr *)&sin, sizeof(sin));
                                sendIndex = 0;
                        } else {
                                v += cplen;
                        }
                }
        }
        return v;

}

