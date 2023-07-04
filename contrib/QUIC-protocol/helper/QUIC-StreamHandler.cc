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
    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->packetToSend = packetTobeSend;

    return Output;
}

} // namespace ns3
