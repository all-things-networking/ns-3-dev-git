#ifndef QUIC_SEND_IF_POSSIBLE_H
#define QUIC_SEND_IF_POSSIBLE_H
#include "../mt-eventprocessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the flow control events.
 */
class QUICSendIfPossible: public MTEventProcessor
{
public:
    QUICSendIfPossible();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};
} // namespace ns3
#endif
