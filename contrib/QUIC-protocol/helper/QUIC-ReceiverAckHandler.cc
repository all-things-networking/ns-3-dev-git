#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-ReceiverAckHandler.h"
#include "QUIC-IntermediateOutput.h"
#include "QUIC-Header.h"

#include "../model/QUIC-Event.h"
#include "../model/QUIC-Context.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max
#include <iostream>

namespace ns3
{

class Packet;
class ModularTransport;


QUICAckHandler::QUICAckHandler():
MTEventProcessor()
{}
bool
QUICAckHandler::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICAckHandler::Process(MTEvent* e, EventProcessorOutput* epOut){

    std::cout << "QUICAckHandler::Process called" << std::endl;
    // az:
    //  if we have received a frame with 
    // we need streamID and offset to send back to sender ACK
    // streamID -> we can get it from either kevin's approach or properly set up
    // offset -> get it from header
    // or fin bit is received

    // to ack on packet, we need the packet number

    ReceivePacketEvent* rpe = static_cast<ReceivePacketEvent*>(e);
    Packet* recvPacket = rpe->receivered;

    //update the context if needed
    MTContext newContext = *(epOut->context);

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent*> newEvents;

    // Create an ACK frame
    ACKFrameFields* fields = new ACKFrameFields();
    fields->LargestACKed = 20;
    QUICFrame ackFrame = QUICFrame(ACK, fields);
    uint8_t buffer[ackFrame.GetSize()];
    // Serialize the ACK frame
    ackFrame.Serialize(buffer);

    // Put frame into packet
    Packet ackPacket = Packet(buffer, ackFrame.GetSize());
    // Add a header for the packet
    MTQUICShortHeader outgoingHeader = MTQUICShortHeader();
    outgoingHeader.pckNum = 20;
    ackPacket.AddHeader(outgoingHeader);

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;
    packetTobeSend.emplace_back(ackPacket);
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}

} // namespace ns3