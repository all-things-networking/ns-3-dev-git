#include "mt-eventprocessor.h"
#include "mt-state.h"
#include "mt-event.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max

namespace ns3
{
class ModularTransport;
//Send if possible, mine
SendIfPossible: SendIfPossible():
MTEventProcessor()
{}
bool
SendSendIfPossible::IsValidEvent(MtEvent e)
{
    MtEvent* ePtr = &e;
    return dynamic_cast<MtCongNotifyEvent*>(ePtr) != nullptr;
}
vector<Packet> SendIfPossible::getpacket(){
    vector<Packet> temp = packetTobeSend.copy();
    packetTobeSend.clear();
    return temp;
}

std::pair<std::vector<MTEvent>, MTContext> SendIfPossible:Process(MTEvent e, MTContext c, ModularTransport *mt){
    //I call mt->SendPack here
    MTContext newContext = c;
    std::vector<MTEvent> newEvents;
    Packet P = new Packet();
    packetTobeSend.emplace_back(P);
    return std::pair<newEvents, c>;
    //store packets to send as vector in class
    //call get packet to retrieve it later, and clear vector in class, use temp vector
}
} // namespace ns3
