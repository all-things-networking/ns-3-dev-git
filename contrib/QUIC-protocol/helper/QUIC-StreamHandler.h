#ifndef QUIC_STREAM_HANDLER_H
#define QUIC_STREAM_HANDLER_H
#include "ns3/mt-eventprocessor.h"
#include "ns3/ipv4-l3-protocol.h"

#include <stdint.h>

#include "QUIC-Frame.h"
#include "../model/QUIC-Event.h"
namespace ns3
{

class MTEvent;
class QUICContext;
class Packet;


/**
 * \brief The processor handling the stream events. This class is not currently used ... maybe can delete this
 */
class QUICStreamHandler: public MTEventProcessor
{
public:
    QUICStreamHandler();
    EventProcessorOutput* Process(MTEvent* e, EventProcessorOutput* epOut);
    bool IsValidEvent(MTEvent * e);

    
};
} // namespace ns3
#endif
