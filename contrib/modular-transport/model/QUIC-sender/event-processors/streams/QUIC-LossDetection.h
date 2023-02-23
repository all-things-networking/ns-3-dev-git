#ifndef QUIC_LOSS_DETECTION_H
#define QUIC_LOSS_DETECTION_H
#include "QUIC-Dispatcher.h"
#include "../QUIC-EventProcessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the loss detection events.
 */
class QUICLossDetection: public QUICEventProcessor
{
public:
    QUICLossDetection();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};
} // namespace ns3
#endif
