#ifndef QUIC_STREAM_PROCESSOR_H
#define QUIC_STREAM_PROCESSOR_H
#include "QUIC-Dispatcher.h"
#include "../QUIC-EventProcessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

/**
 * \brief The processor handling the stream events.
 */
class QUICStreamProcessor: public QUICEventProcessor
{
public:
    QUICStreamProcessor();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
};
} // namespace ns3
#endif
