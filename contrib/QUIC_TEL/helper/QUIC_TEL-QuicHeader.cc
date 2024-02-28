#include "QUIC_TEL-QuicHeader.h"
#include <iostream>
using namespace std;

namespace ns3
{
    QuicHeader::QuicHeader(int pkt_id)
    {
        this->pkt_id = pkt_id;
    };
    uint32_t QuicHeader::GetSerializedSize() {
        return 0;
    }
    void QuicHeader::Serialize(Buffer::Iterator start) {}
    uint32_t QuicHeader::Deserialize(Buffer::Iterator start) {
        return 0;
    }
    void QuicHeader::Print(std::ostream& os) {}
} // namespace ns3