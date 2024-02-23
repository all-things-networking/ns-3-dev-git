#ifndef ITERM_OUT
#define ITERM_OUT
#include <vector>

namespace ns3 {
    class iterm_out {
        public:
        pkt_t packet; //this is currently not defined
        PacketInfo info;
        vector<PacketInfo *> lost_packet;
        iterm_out(pkt_t packet, PacketInfo info);
    };  
}
#endif