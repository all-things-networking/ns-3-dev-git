#include "../model/QUIC-Context.h"
#include "../model/QUIC-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-state.h"
#include "ns3/node.h"

#include "QUIC-Stream.h"
#include "QUIC-LossDetection.h"

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
    MTContext * c = epOut->context;

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
    Output->context = newContext;
    Output->packetToSend = packetTobeSend;
    return Output;
}

EventProcessorOutput* QUICLossDetection::HandleReceiveACK(ResponseEvent* e, QUICContext* c){

    // Update the state of the sent packet to ACKED
    ResponseEventData responseData = e->data;
    std::deque<std::pair<Ptr<Packet>, PacketState>> sentPackets = c->sentPackets;

    // ACKed packed be in range [sendBase, sendBase + windowSize]
    if (responseData.packetNum >= c->sendBase && responseData.packetNum <= c->sendBase + c->windowSize) {
        if (responseData.packetNum == c->sendBase) {
            c->sendBase += 1;
            c->sentPackets.pop_front();
        }
        else {
            sentPackets.at(responseData.packetNum).second = PacketState::ACKED;
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
        // WIP, need to seperate into data again and add the data to head of data buffers for retransmission

        uint8_t* buffer = new uint8_t[lostPacket->GetSize()];
        int size = lostPacket->CopyData(buffer, lostPacket->GetSize());
        std::string s = std::string(buffer, buffer + lostPacket->GetSize());

        std::cout << "Retransmitting: " << s << std::endl;

        std::stringstream retransmitPacket(s);
        std::string frame;

        while(std::getline(retransmitPacket, frame, '_'))
        {
            QUICFrame* dataFrame = new QUICFrame(frame);
            StreamFrameFields* streamFrameFields = static_cast<StreamFrameFields*>(dataFrame->fields);
            QUICStream* stream = c->quic_streams[streamFrameFields->StreamID];

            // Add to front of stream data buffer
            stream->databuffer = dataFrame->GetData() + stream->databuffer;
        }
    }
    

    // Empty Event
    std::vector<MTEvent*> newEvents;
    std::vector<Packet> packetTobeSend;

    // Output
    EventProcessorOutput* Output = new EventProcessorOutput;
    Output->newEvents = newEvents;
    Output->context = c;
    Output->packetToSend = packetTobeSend;
    return Output;
}


} // namespace ns3
