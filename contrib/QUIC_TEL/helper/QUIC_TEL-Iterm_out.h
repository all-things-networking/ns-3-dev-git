#ifndef ITERM_OUT
#define ITERM_OUT
#include "ns3/node.h"
#include "QUIC_TEL-PacketInfo.h"

#include <vector>

namespace ns3 {
    class iterm_out {
        public:
        Packet* packet;
        PacketInfo info;
        std::vector<PacketInfo *> lost_packet;
        iterm_out(Packet* packet, PacketInfo info);
    };  
}
#endif