#include "QUIC-receivelogic.h"
#include "mt-receivelogic.h"
#include "modular-transport.h"
#include "mt-scheduler.h"
#include "mt-event.h"
#include "QUIC-event.h"
#include "QUIC-header.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include <stdlib.h>
namespace ns3{

QUICReceiveLogic::QUICReceiveLogic(){
    this->next_acknum = 0;
}
enum IpL4Protocol::RxStatus QUICReceiveLogic::Receive(ModularTransport* mt,
                                    Ptr<Packet> packet,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface){

    MTQUICShortHeader recievedHeader;
    packet->RemoveHeader(recievedHeader);
    //chosenScheduler.OpsAfterRecieved(recievedHeader);
    //chosenScheduler.GenerateEventOnReceive(recievedHeader);
    //recievedHeader.OpsAfterRecieved(); //THis one returns a event
    NS_LOG_UNCOND(recievedHeader);
    uint8_t* buffer = new uint8_t[packet->GetSize()];
    packet->CopyData(buffer, packet->GetSize());
    
    NS_LOG_UNCOND("Received packet in ModularTransport");
    NS_LOG_UNCOND("Content of the packet:");
    NS_LOG_UNCOND((int)buffer[packet->GetSize()-1]);
    return IpL4Protocol::RX_OK;
}
}