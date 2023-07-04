#include "QUIC-StreamHandler.h"
#include "QUIC-Stream.h"
#include "QUIC-SendPacket.h"
#include "QUIC-AddStreamData.h"

#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"

#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"

#include <algorithm> // std::min, std::max
#include <iostream>
#include <utility> // std::pair
#include <vector>

namespace ns3
{

// TODO: This file needs to be adapted for QUIC.

class ModularTransport;
class StreamEvent;

// Send if possible, mine
QUICStreamHandler::QUICStreamHandler()
    : MTEventProcessor()
{
}

bool
QUICStreamHandler::IsValidEvent(MTEvent * e)
{
    return true;
}

EventProcessorOutput*
QUICStreamHandler::Process(MTEvent* e, EventProcessorOutput* epOut)
{
    MTContext * c = epOut->context;

    // I call mt->SendPack here
    QUICContext* newContext = dynamic_cast<QUICContext*>(c);
    StreamEvent* streamEvent = dynamic_cast<StreamEvent*>(e);

    // If we have a SEND_PACKET event we will send the packet
    if (streamEvent->streamEventType == StreamEventType::SEND_PACKET)
    {
        QUICSendPacket processor;
        EventProcessorOutput* res = processor.TrySendPacket(streamEvent, newContext);
        return res;
    }

    // If we have a ADD_DATA event, create a dataFrame and send it
    if (streamEvent->streamEventType == StreamEventType::ADD_DATA)
    {
        QUICAddStreamData processor;
        EventProcessorOutput* res = processor.TryAddStreamData(streamEvent, newContext);
        return res;
    }

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->context = newContext;
    Output->packetToSend = packetTobeSend;

    return Output;
}

} // namespace ns3
