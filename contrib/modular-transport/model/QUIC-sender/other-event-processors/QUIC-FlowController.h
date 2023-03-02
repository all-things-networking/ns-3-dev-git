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
private:
    int maxData;
    int maxStreamData;
public:
    QUICFlowController();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
    void SetMaxStreamData(int value);
    void SetMaxData(int value);
    int GetMaxStreamData();
    int GetMaxData();
};
} // namespace ns3
#endif
