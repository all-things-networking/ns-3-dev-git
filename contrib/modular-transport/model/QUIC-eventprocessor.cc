#include "QUIC-eventprocessor.h"
#include "mt-state.h"
#include "QUIC-event.h"
#include "QUIC-context.h"
#include "QUIC-intermediate_output.h"
#include "QUIC-header.h"
#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

namespace ns3
{

class Packet;
class ModularTransport;


QUICAppCommunication::QUICAppCommunication():
MTEventProcessor()
{}
bool
QUICAppCommunication::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICAppCommunication::Process(MTEvent* e, EventProcessorOutput* epOut){

    //update the context if needed
    MTContext newContext = *(epOut->context);

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent*> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}


QUICPacketDemultiplexer::QUICPacketDemultiplexer():
MTEventProcessor()
{}
bool
QUICPacketDemultiplexer::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICPacketDemultiplexer::Process(MTEvent* e, EventProcessorOutput* epOut){
    ReceivePacketEvent* rpe = static_cast<ReceivePacketEvent*>(e);
    recvPacket = rpe->receivered;

    demultiplexePacket();

    //update the context if needed
    MTContext newContext = *epOut->context;

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent*> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;

    QUICIntermediateOutput intermOutput;
    intermOutput.PacketDemultiplexerOut = FrameToStream;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context=&newContext;
        Output->packetToSend=packetTobeSend;
        Output->intermOutput = intermOutput;
    return Output;
}

void QUICPacketDemultiplexer::demultiplexePacket() {
    // process recvPacket, fill FrameToStream
    return;
}



QUICBufferManagement::QUICBufferManagement():
MTEventProcessor()
{}
bool
QUICBufferManagement::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICBufferManagement::Process(MTEvent* e, EventProcessorOutput* epOut){

    QUICContext* qc = static_cast<QUICContext*>(epOut->context);
    std::vector<QUICStream> streams = qc->streams;
    // check the buffer for each stream, and the whole connection

    //update the context if needed
    MTContext newContext = *(epOut->context);

    std::vector<MTEvent*> newEvents;

    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}


QUICAckHandler::QUICAckHandler():
MTEventProcessor()
{}
bool
QUICAckHandler::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICAckHandler::Process(MTEvent* e, EventProcessorOutput* epOut){

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