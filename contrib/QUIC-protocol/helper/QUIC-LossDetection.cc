#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"

#include "QUIC-Stream.h"
#include "QUIC-LossDetection.h"
#include "QUIC-HandleReceiveACK.h"

#include <algorithm> // std::min, std::max
#include <iostream>
#include <utility> // std::pair
#include <vector>
#include <sstream>
namespace ns3
{

// TODO: This file needs to be adapted for QUIC.

class ModularTransport;
class ResponseEvent;

// Send if possible, mine
QUICLossDetection::QUICLossDetection()
    : MTEventProcessor()
{
}

bool
QUICLossDetection::IsValidEvent(MTEvent * e)
{
    return true;
}

EventProcessorOutput*
QUICLossDetection::Process(MTEvent* e, EventProcessorOutput* epOut)
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
