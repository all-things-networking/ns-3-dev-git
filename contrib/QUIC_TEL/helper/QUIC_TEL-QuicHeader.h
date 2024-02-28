#include "ns3/header.h"

namespace ns3
{
    class QuicHeader : public Header {
        public:
            int pkt_id;
            QuicHeader(int pkt_id);
            uint32_t GetSerializedSize() const;
            void Serialize(Buffer::Iterator start) const;
            uint32_t Deserialize(Buffer::Iterator start);
            void Print(std::ostream& os) const;
            TypeId GetInstanceTypeId() const;
    };
} // namespace ns3