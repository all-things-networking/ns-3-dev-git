#include "TCP-receivelogic.h"
#include "mt-receivelogic.h"
#include "modular-transport.h"
#include "mt-scheduler.h"
#include "mt-event.h"
#include "TCP-event.h"
#include "TCP-header.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3{

TCPReceiveLogic::TCPReceiveLogic(){

}
enum IpL4Protocol::RxStatus TCPReceiveLogic::Receive(ModularTransport* mt,
                                    Ptr<Packet> packet,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface){
    std::cout<<"inside received"<<std::endl;
    MTTCPHeader recievedHeader;
    MTTCPHeader headerType;
    packet->RemoveHeader(recievedHeader);
    std::cout<<"seqnum"<<recievedHeader.seqnum<<std::endl;
    uint8_t *buffer = new uint8_t[packet->GetSize()];
    uint32_t size = packet->CopyData(buffer, packet->GetSize());
    std::cout<<"Received: size"<<size<<std::endl;
        for(int i=0;i<packet->GetSize();i++){
            std::cout<<unsigned(buffer[i])<<std::endl;
        }
        //chosenScheduler.OpsAfterRecieved(recievedHeader);
        //chosenScheduler.GenerateEventOnReceive(recievedHeader);
        //recievedHeader.OpsAfterRecieved(); //THis one returns a event
    std::cout<<mt << packet << incomingIpHeader << incomingInterface<<std::endl;

    std::cout<<"Received packet in ModularTransport"<<std::endl;
    if((recievedHeader.ControlBits&headerType.ackbit) > 0){
            std::cout<<"Ack recevied from"<<std::endl;
            std::cout<<incomingIpHeader.GetSource()<<std::endl;
            //need to implement: int flow_id, int seq
             //MTEvent* e = mt->scheduler->CreateAckEvent(1,recievedHeader.seqnum + size);
             MTEvent* e = mt->scheduler->CreateAckEvent(1,recievedHeader.acknum);
             mt->scheduler->AddEvent(e);
    }
    else{
            std::cout<<"Sending back Ack"<<std::endl;
            Packet P = Packet();
            MTTCPHeader ACKHeader;
            ACKHeader.acknum = recievedHeader.seqnum + size;
            ACKHeader.seqnum = recievedHeader.seqnum + size;
            ACKHeader.ControlBits|= headerType.ackbit;
            P.AddHeader(ACKHeader);
            //recreate Header for outgoing
            mt->SendPacket(&P, incomingIpHeader.GetDestination(), incomingIpHeader.GetSource());
        }
    return IpL4Protocol::RX_OK;
}
}
