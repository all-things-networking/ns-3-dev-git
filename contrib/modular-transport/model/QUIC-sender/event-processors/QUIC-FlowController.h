#ifndef QUIC_FLOW_CONTROLLER_H
#define QUIC_FLOW_CONTROLLER_H
#include "QUIC-Dispatcher.h"
#include "QUIC-EventProcessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the flow control events.
 */
class QUICFlowController: public QUICEventProcessor
{
public:
    QUICFlowController();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};
} // namespace ns3
#endif
