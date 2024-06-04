#include "QUIC_TEL-Iterm_out.h"

namespace ns3 {
    QUICIntermediateOutput::QUICIntermediateOutput(Packet* packet, PacketInfo info) {
        this->packet = packet;
        this->info = info;
    };
}