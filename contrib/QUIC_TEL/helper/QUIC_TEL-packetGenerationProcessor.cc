#include "QUIC_TEL-sendProcessor.h"
#include "ns3/timer.h"

namespace ns3
{
    EventProcessorOutput* packetGenerationProcessor::Process(MTEvent* e, EventProcessorOutput* epOut) {
        return nullptr;
    }

    packetGenerationProcessor::packetGenerationProcessor(){}

    packetGenerationProcessor::~packetGenerationProcessor(){}

    bool packetGenerationProcessor::IsValidEvent(MTEvent* e) { 
        return true;
    }

    void packetGenerationProcessor::own_Process(QUICEvent* e, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out) {
        Packet new_pkt;
        
        PacketInfo pkt_info;
        pkt_info.time_sent = 0;
        QuicHeader qheader;
        qheader.pkt_id = ctx->id_counter++;
        pkt_info.packet_id = qheader.packet_id;
        new_pkt.AddHeader(qheader)
        ctx->sent_packets.emplace_back(&pkt_info);
        // to add: tx_module implementation
    }
}
