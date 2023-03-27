#include "TCP-receivelogic.h"
#include "mt-receivelogic.h"
#include "modular-transport.h"
#include "mt-scheduler.h"
#include "mt-event.h"
#include "TCP-event.h"
#include "TCP-header.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include <stdlib.h>
namespace ns3{

TCPReceiveLogic::TCPReceiveLogic(){
    this->next_acknum = 0;
}
enum IpL4Protocol::RxStatus TCPReceiveLogic::Receive(ModularTransport* mt,
                                    Ptr<Packet> packet,
                                    const Ipv4Header& incomingIpHeader,
                                    Ptr<Ipv4Interface> incomingInterface){

    MTTCPHeader recievedHeader;
    MTTCPHeader headerType;
    packet->RemoveHeader(recievedHeader);
    int drop = rand()%10;
    if(drop > 7 && (recievedHeader.ControlBits&headerType.ackbit) == 0){
        //8 , 9 20% chance
        //on receiver side
        std::cout<<"Receiver dropped"<<" "<<recievedHeader.seqnum<<std::endl;
        return IpL4Protocol::RX_OK;
    }
    uint8_t *buffer = new uint8_t[packet->GetSize()];
    uint32_t size = packet->CopyData(buffer, packet->GetSize());
    std::cout<<size<<std::endl;
    std::cout<<"Received: size"<<size<<std::endl;
    for(int i=0;i<packet->GetSize();i++){
        std::cout<<unsigned(buffer[i])<<std::endl;
    }
    //chosenScheduler.OpsAfterRecieved(recievedHeader);
    //chosenScheduler.GenerateEventOnReceive(recievedHeader);
    //recievedHeader.OpsAfterRecieved(); //THis one returns a event
    //std::cout<<mt << packet << incomingIpHeader << incomingInterface<<std::endl;

    //std::cout<<"Received packet in ModularTransport"<<std::endl;
    if((recievedHeader.ControlBits&headerType.ackbit) > 0){
            std::cout<<"Ack recevied for acknum"<<recievedHeader.acknum<<std::endl;
            //need to implement: int flow_id, int seq
             //MTEvent* e = mt->scheduler->CreateAckEvent(1,recievedHeader.seqnum + size);
             MTEvent* e = mt->scheduler->CreateAckEvent(1,recievedHeader.acknum);
             mt->scheduler->AddEvent(e);
    }
    else{
            std::cout<<"Receiver Sending back Ack for acknum"<<" "<<recievedHeader.seqnum<<std::endl;
            Packet P = Packet();

            this->packetbuffer[recievedHeader.seqnum] = size;
            while(this->packetbuffer.count(next_acknum)){
            //0 1
                std::cout<<(uint32_t)packetbuffer[this->next_acknum]<<" "<<this->packetbuffer.count(next_acknum)<<std::endl;
                this->next_acknum += (uint32_t)(this->packetbuffer[this->next_acknum]);
            }
            MTTCPHeader ACKHeader;
            ACKHeader.acknum = this->next_acknum;
            ACKHeader.seqnum = this->next_acknum;
            ACKHeader.ControlBits|= headerType.ackbit;
            P.AddHeader(ACKHeader);
            //recreate Header for outgoing
            mt->SendPacket(&P, incomingIpHeader.GetDestination(), incomingIpHeader.GetSource());
        }
    return IpL4Protocol::RX_OK;
}
}
