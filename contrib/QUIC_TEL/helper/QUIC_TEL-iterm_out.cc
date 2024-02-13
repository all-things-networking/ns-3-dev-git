#include "mt-intermediateoutput.h"

#include <utility> // std::pair
#include <vector>

namespace ns3 {
    iterm_out::iterm_out(pkt_t packet, PacketInfo info) {
        this->packet = packet;
        this->info = info;
    };
}