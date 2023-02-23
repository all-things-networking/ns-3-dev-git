#ifndef QUIC_CONGESTION_CONTROLLER_H
#define QUIC_CONGESTION_CONTROLLER_H
#include "QUIC-Dispatcher.h"
#include "QUIC-EventProcessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the congestion control events.
 */
class QUICCongestionController: public QUICEventProcessor
{
public:
    QUICCongestionController();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};
} // namespace ns3
#endif
