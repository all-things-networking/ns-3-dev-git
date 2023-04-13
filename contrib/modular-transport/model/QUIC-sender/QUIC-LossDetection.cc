#include "QUIC-LossDetection.h"

#include "../QUIC-Context.h"
#include "../QUIC-Event.h"
#include "../mt-event.h"
#include "../mt-eventprocessor.h"
#include "../mt-state.h"
#include "QUIC-Stream.h"

#include "ns3/node.h"

#include <algorithm> // std::min, std::max
#include <iostream>
#include <utility> // std::pair
#include <vector>

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
QUICLossDetection::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput*
QUICLossDetection::Process(MTEvent* e, MTContext* c)
{
    // I call mt->SendPack here
    QUICContext* newContext = dynamic_cast<QUICContext*>(c);
    ResponseEvent* responseEvent = dynamic_cast<ResponseEvent*>(e);

    // If we have a SEND_PACKET event we will send the packet
    if (responseEvent->responseEventType == ResponseEventType::ACK_PACKET)
    {
        EventProcessorOutput* res = HandleReceiveACK(responseEvent, newContext);
        return res;
    }

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->updatedContext = newContext;
    Output->packetToSend = packetTobeSend;
    return Output;
}

EventProcessorOutput* QUICLossDetection::HandleReceiveACK(ResponseEvent* e, QUICContext* c){

    // Update the state of the sent packet to ACKED
    ResponseEventData responseData = e->data;
    std::deque<std::pair<Ptr<Packet>, PacketState>> sentPackets = c->sentPackets;

    // Should be in range
    if (responseData.packetNum >= c->sendBase && responseData.packetNum <= c->sendBase + c->windowSize) {
        if (responseData.packetNum == c->sendBase) {
            c->sendBase += 1;
            c->sentPackets.pop_front();
        }
        else {
            sentPackets.at(responseData.packetNum).second = PacketState::ACKED;
            // WIP need to update frame buffers to acked
        }
    }

    /*
    [RFC 9002]
    [https://www.rfc-editor.org/rfc/rfc9002#name-packet-threshold]

    Every packetNum that is between k_packet_threshold from current acked packet is considered lost
    */

    int lostIndex = responseData.packetNum - c->k_packet_threshold - 1;
    while (lostIndex > c->sendBase) {
        Ptr<Packet> lostPacket = sentPackets.at(responseData.packetNum).first;
        // WIP, need to seperate into frames again and add the frames to head of frame buffers for retransmission

    }
    

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->updatedContext = c;
    Output->packetToSend = packetTobeSend;
    return Output;
}


} // namespace ns3
