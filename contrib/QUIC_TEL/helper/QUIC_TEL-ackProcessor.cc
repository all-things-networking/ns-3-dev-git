#include "QUIC_TEL-ackProcessor.h"
#include "QUIC_TEL-Frame.h"
#include "ns3/mt-eventprocessor.h"
#include <map>

namespace ns3
{

QUICAckProcessor::QUICAckProcessor()
{
}

QUICAckProcessor::~QUICAckProcessor()
{
}

bool QUICAckProcessor::IsValidEvent(MTEvent * e)
{
    return true;
}

void
QUICAckProcessor::Process(AckEvent* ev, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out)
{
    if (ctx->largest_acked_packet == -1) {
        ctx->largest_acked_packet = ev->largest_acked;
    } else {
        ctx->largest_acked_packet = std::max(ctx->largest_acked_packet, ev->largest_acked);
    }
    bool new_packet_acked = false;
    std::vector<PacketInfo *> acked_packets;
    int acked_bytes;
    for (int i = 0; i < ev->packet_nums.size(); i++) {
        for (int j = 0; j < ctx->sent_packets.size(); j++) {
            if (i == ctx->sent_packets[j]->packet_id) {
                if (i == ctx->largest_acked_packet) {
                    ctx->largest_acked_time = ctx->sent_packets[j]->time_sent;
                }
                acked_packets.emplace_back(ctx->sent_packets[j]);
                ctx->acked_bytes -= ctx->sent_packets[i].size();
                ctx->sent_packets.erase(j);
                new_packet_acked = true;
                break;
            }
        }
    }
    ctx->bytes_in_flight -= acked_bytes;
    if (!new_packet_acked) {
        return;
    }
    ctx.latest_rtt = Simulator:: - ctx->largest_acked_time;
    ctx->update_rtt = true;

    if (ev->ECN_CE_Count > ctx->ecn_ce_counters) {
        ctx->ecn_ce_counters = ack->ECN_CE_Count;
        out->in_congestion = true;
        out->start_time = ctx->largest_acked_packet;
    }
}


} // namespace ns3
