#ifndef ITERM_OUT
#define ITERM_OUT
#include "ns3/node.h"
#include "QUIC_TEL-PacketInfo.h"
#include "ns3/mt-intermediateoutput.h"

#include <vector>

namespace ns3 {
    class QUICIntermediateOutput :  public MTIntermediateOutput {
        public:
        Packet* packet;
        PacketInfo info;
        std::vector<PacketInfo *> lost_packet;
        QUICIntermediateOutput(Packet* packet, PacketInfo info);
    };  
}
#endif