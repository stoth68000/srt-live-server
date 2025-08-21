#ifndef _TransportStatistics_INCLUDE_
#define _TransportStatistics_INCLUDE_

#include <libltntstools/ltntstools.h>

class CTransportStatistics
{
public :
	CTransportStatistics();
	~CTransportStatistics();

	int write(unsigned char *pkts, int packetCount);
	uint64_t get_iat_hwm_us();
	uint64_t get_iat_hwm_ms();

	struct ltntstools_stream_statistics_s *m_stats;
private:
	int m_updates;
	void *m_streammodel;
	struct ltntstools_pat_s *m_pat;
	struct ltn_histogram_s *m_iat;
};

#endif
