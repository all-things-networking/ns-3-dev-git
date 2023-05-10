#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-BufferManagement.h"
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
} // namespace ns3