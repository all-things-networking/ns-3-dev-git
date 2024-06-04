#include "QUIC_TEL-sendProcessor.h"
#include "ns3/timer.h"

namespace ns3
{
    EventProcessorOutput* QUICSendProcessor::Process(MTEvent* e, EventProcessorOutput* epOut) {
            // TODO: Discuss where the timer comes from in quicNew.tel (Line 250)
        if (epOut->intermOutput->info.in_flight) {
            if (epOut->intermOutput->info.ack_eliciting) {
                epOut->ctx->time_of_last_ack_eliciting_packet = 0;
            }
        }
        return epOut; // ? 
    }

    QUICSendProcessor::QUICSendProcessor(){}

    QUICSendProcessor::~QUICSendProcessor(){}

    bool QUICSendProcessor::IsValidEvent(MTEvent* e) { 
        return true;
    }

    void QUICSendProcessor::own_Process(QUICEvent* e, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out) {
    
}
