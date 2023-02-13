#include "mt-eventprocessor.h"
#include "mt-state.h"
#include "mt-event.h"
#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max

namespace ns3
{
class Packet;
class ModularTransport;
//Send if possible, mine
SendIfPossible::SendIfPossible():
MTEventProcessor()
{}
bool
SendSendIfPossible::IsValidEvent(MTEvent e)
{
    MTEvent* ePtr = &e;
    return true;
}
std::vector<Packet> SendIfPossible::getpacket(){
    vector<Packet> temp = packetTobeSend.copy();
    packetTobeSend.clear();
    return temp;
}

std::pair<std::vector<MTEvent>, MTContext> SendIfPossible::Process(MTEvent e, MTContext c){
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
