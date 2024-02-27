#include "QUIC-sendProcessor.h"

namespace ns3
{

    QUICSendProcessor::QUICSendProcessor(){}

    QUICSendProcessor::~QUICSendProcessor(){}

    bool QUICSendProcessor::IsValidEvent(QUICEvent* e) { 
        return true;
    }

    void QUICSendProcessor::Process(QUICEvent* e, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out) {
        if (out->info.in_flight) {
            if (out->info.ack_eliciting) {
                ctx->time_of_last_ack_eliciting_packet = time(0);
            }
        }
    }
}
