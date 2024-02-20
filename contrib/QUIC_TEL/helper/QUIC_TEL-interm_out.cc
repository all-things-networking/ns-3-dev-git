#include "QUIC_TEL-interm_out.h"

#include <vector>

namespace ns3 {
    interm_out::interm_out(pkt_t packet, PacketInfo info) {
        this->packet = packet;
        this->info = info;
    };
}