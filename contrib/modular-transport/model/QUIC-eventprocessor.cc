#include "QUIC-eventprocessor.h"
#include "mt-state.h"
#include "QUIC-event.h"
#include "QUICPacket.h"
#include "QUIC-context.h"
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
QUICAppCommunication::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* QUICAppCommunication::Process(MTEvent e, MTContext c){

    //update the context if needed
    MTContext newContext = c;

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->updatedContext=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}


QUICPacketDemultiplexer::QUICPacketDemultiplexer():
MTEventProcessor()
{}
bool
QUICPacketDemultiplexer::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* QUICPacketDemultiplexer::Process(MTEvent e, MTContext c){

    ReceivePacketEvent rpe = static_cast<ReceivePacketEvent&>(e);
    recvPacket = rpe.receivered;

    demultiplexePacket();
    processFrame(c);

    //update the context if needed
    MTContext newContext = c;

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->updatedContext=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}

void QUICPacketDemultiplexer::demultiplexePacket() {
    // process recvPacket, fill FrameToStream
}

void QUICPacketDemultiplexer::processFrame(MTContext c) {
    QUICContext qc = static_cast<QUICContext&>(c);
    // process each frame
}



QUICBufferManagement::QUICBufferManagement():
MTEventProcessor()
{}
bool
QUICBufferManagement::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* QUICBufferManagement::Process(MTEvent e, MTContext c){

    QUICContext qc = static_cast<QUICContext&>(c);
    std::vector<QUICStream> streams = qc.streams;
    // check the buffer for each stream, and the whole connection

    //update the context if needed
    MTContext newContext = c;

    std::vector<MTEvent> newEvents;

    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->updatedContext=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}


QUICAckHandler::QUICAckHandler():
MTEventProcessor()
{}
bool
QUICAckHandler::IsValidEvent(MTEvent e)
{
    return true;
}

EventProcessorOutput* QUICAckHandler::Process(MTEvent e, MTContext c){

    ReceivePacketEvent rpe = static_cast<ReceivePacketEvent&>(e);
    QUICPacket recvPacket = rpe.receivered;

    //update the context if needed
    MTContext newContext = c;

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->updatedContext=&newContext;
        Output->packetToSend=packetTobeSend;
    return Output;
}


} // namespace ns3