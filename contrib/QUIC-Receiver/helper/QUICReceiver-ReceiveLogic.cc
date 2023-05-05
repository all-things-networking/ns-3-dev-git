#include "ns3/mt-receivelogic.h"
#include "ns3/modular-transport.h"
#include "ns3/mt-scheduler.h"
#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "../model/QUICReceiver-Event.h"
#include "QUICReceiver-Header.h"
#include "QUICReceiver-ReceiveLogic.h"

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