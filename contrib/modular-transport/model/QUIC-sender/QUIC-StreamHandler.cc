#include "QUIC-StreamHandler.h"

#include "../QUIC-Context.h"
#include "../mt-event.h"
#include "../QUIC-Event.h"
#include "../mt-eventprocessor.h"
#include "../mt-state.h"

#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "QUIC-StreamContext.h"

#include <algorithm> // std::min, std::max
#include <utility>   // std::pair
#include <vector>
#include <iostream>

namespace ns3
{

// TODO: This file needs to be adapted for QUIC.

class Packet;
class ModularTransport;

// Send if possible, mine
QUICStreamHandler::QUICStreamHandler()
    : MTEventProcessor()
{
}

bool
QUICStreamHandler::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput*
QUICStreamHandler::Process(MTEvent* e, MTContext* c)
{
    // I call mt->SendPack here
    QUICContext* newContext = dynamic_cast<QUICContext*>(c);
    StreamEvent* streamEvent = dynamic_cast<StreamEvent*>(e);
    int stream_id = streamEvent->stream_id;

    std::cout << "STREAM ID HERE" << std::endl;
    std::cout << streamEvent->stream_id << std::endl;

    // Check if stream (stream_id) exists, if not create it
    auto it = newContext->quic_streams.find(stream_id);
    if (it == newContext->quic_streams.end()) {
        // TODO: need to free this?
        QUICStreamContext* steamContext = new QUICStreamContext(stream_id);
        newContext->quic_streams[stream_id] = steamContext;
        std::cout << "Created new stream with ID: " << newContext->quic_streams.find(stream_id)->second->id << std::endl;
    }
    
    // A
    std::vector<MTEvent*> newEvents;

    // New Packets
    // Add window
    std::vector<Packet> packetTobeSend;
    Packet P = Packet(newContext->data, 4);
    packetTobeSend.emplace_back(P);

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->updatedContext = newContext;
    Output->packetToSend = packetTobeSend;
    return Output;
    // store packets to send as vector in class
    // call get packet to retrieve it later, and clear vector in class, use temp vector
}
} // namespace ns3
