#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-PacketDemultiplexer.h"
#include "QUIC-IntermediateOutput.h"
#include "QUIC-Header.h"

#include "../model/QUIC-Event.h"
#include "../model/QUIC-Context.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max

namespace ns3
{

class Packet;
class ModularTransport;


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
} // namespace ns3