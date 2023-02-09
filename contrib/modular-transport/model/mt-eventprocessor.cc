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
MtEventProcessor()
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

std::pair<std::vector<MtEvent>, MtContext> SendIfPossible:Process(MtEvent e, MtContext c, ModularTransport *mt){
    //I call mt->SendPack here
    MtContext newContext = c;
    std::vector<MtEvent> newEvents;
    Packet P = new Packet();
    packetTobeSend.emplace_back(P);
    return std::pair<newEvents, c>;
    //store packets to send as vector in class
    //call get packet to retrieve it later, and clear vector in class, use temp vector
}
} // namespace ns3
