#include "QUIC_TEL-Iterm_out.h"
#include <vector>

namespace ns3 {
    iterm_out::iterm_out(pkt_t packet, PacketInfo info) {
        this->packet = packet;
        this->info = info;
    };
}