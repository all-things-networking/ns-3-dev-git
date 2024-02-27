#include "QUIC_TEL-QuicHeader.h"

namespace ns3
{
    QuicHeader::QuicHeader(int pkt_id)
    {
        this->pkt_id = pkt_id;
    };
} // namespace ns3