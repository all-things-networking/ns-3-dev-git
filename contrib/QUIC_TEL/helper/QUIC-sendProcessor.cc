#include "QUIC-sendProcessor.h"
#include "QUIC_TEL-Frame.h"
#include "ns3/mt-eventprocessor.h"
#include <ctime>
#include <map>

namespace ns3
{

QUICSendProcessor::QUICSendProcessor()
{
}

QUICSendProcessor::~QUICSendProcessor()
{
}

bool
QUICSendProcessor::IsValidEvent(MTEvent* e)
{
    return true;
}


void
QUICSendProcessor::Process(QUICEvent* e, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out)
{
    if (out->info.in_flight) {
        if (out->info.ack_eliciting) {
            ctx->time_of_last_ack_eliciting_packet = time(0);
        }
    }

}
} // namespace ns3
