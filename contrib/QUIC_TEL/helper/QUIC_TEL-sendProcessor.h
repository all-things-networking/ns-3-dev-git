#ifndef QUIC_SEND_PACKET_H
#define QUIC_SEND_PACKET_H

#include "QUIC_TEL-Iterm_out.h"
#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-eventprocessor.h"

namespace ns3
{
    class QUICSendProcessor : public MTEventProcessor {
        public:
        QUICSendProcessor();
        ~QUICSendProcessor();

        EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
        // empty, only for inherit
        void own_Process(QUICEvent* e, QUICContext *ctx, std::vector<QUICEvent *> events, std::vector<Packet *> packets, iterm_out *out);
        bool IsValidEvent(MTEvent* e);
    };
}

#endif
