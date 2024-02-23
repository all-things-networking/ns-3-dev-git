#include <utility> // std::pair
#include <vector>

namespace ns3
{
    class QuicHeader
    {
    public:
        int pkt_id;
        QuicHeader(int pkt_id);
    };
} // namespace ns3