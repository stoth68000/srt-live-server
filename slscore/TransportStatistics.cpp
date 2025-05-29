#include <errno.h>
#include <string.h>

#include "TransportStatistics.hpp"

CTransportStatistics::CTransportStatistics()
{
	m_stats = NULL;
	m_updates = 0;
	m_pat = NULL;
	m_iat = NULL;

	ltntstools_pid_stats_alloc(&m_stats);
	ltntstools_pid_stats_reset(m_stats);

	ltntstools_streammodel_alloc(&m_streammodel, this);

	ltn_histogram_alloc_video_defaults(&m_iat, "Stream IAT");
}

CTransportStatistics::~CTransportStatistics()
{
	if (m_streammodel) {
		ltntstools_streammodel_free(m_streammodel);
		m_streammodel = NULL;
	}

	if (m_stats) {
		ltntstools_pid_stats_free(m_stats);
		m_stats = NULL;
	}

	if (m_pat) {
		ltntstools_pat_free(m_pat);
		m_pat = NULL;
	}

	if (m_iat) {
		ltn_histogram_free(m_iat);
		m_iat = NULL;
	}
}

int CTransportStatistics::write(unsigned char *pkts, int packetCount)
{
	struct timeval now;
	gettimeofday(&now, NULL);

	ltntstools_pid_stats_update(m_stats, pkts, packetCount);
	ltn_histogram_interval_update(m_iat, &now);

	int complete = 0;
	size_t s = ltntstools_streammodel_write(m_streammodel, (const unsigned char *)pkts, packetCount, &complete, &now);

	if (complete) {
		if (m_pat) {
			ltntstools_pat_free(m_pat);
			m_pat = NULL;
		}
		if (ltntstools_streammodel_query_model(m_streammodel, &m_pat) == 0) {
			ltntstools_pat_dprintf(m_pat, 1);
		}
	}

	if (m_updates++ % 4096 == 0) {
#if 0
		ltntstools_pid_stats_dprintf(m_stats, 1);
		ltn_histogram_interval_print(1, m_iat, 0);
#endif
	}
}

