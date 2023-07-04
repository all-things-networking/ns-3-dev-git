#include "QUIC-ReceiveLogic.h"

#include "ns3/modular-transport.h"
#include "ns3/mt-receiver.h"

#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include <string>

namespace ns3
{

QUICReceiveLogic::QUICReceiveLogic()
{
}

enum IpL4Protocol::RxStatus
QUICReceiveLogic::Receive(ModularTransport* mt,
                      Ptr<Packet> packet,
                      const Ipv4Header& incomingIpHeader,
                      Ptr<Ipv4Interface> incomingInterface)
{
    std::cout << "########### Received ###########" << std::endl;
    ;
    MTHeader recievedHeader;
    packet->RemoveHeader(recievedHeader);
    uint8_t* buffer = new uint8_t[packet->GetSize()];
    int size = packet->CopyData(buffer, packet->GetSize());

    //
    std::string s = std::string(buffer, buffer + packet->GetSize());
    std::cout << "Received: " << s << std::endl;
    std::cout << "Length: " << packet->GetSize() << std::endl;

    // chosenScheduler.OpsAfterRecieved(recievedHeader);
    // chosenScheduler.GenerateEventOnReceive(recievedHeader);
    // recievedHeader.OpsAfterRecieved(); //THis one returns a event
    std::cout << mt << packet << incomingIpHeader << incomingInterface << std::endl;

    // Right now we are just hard-coding a response ACK for testing purposes
    std::cout << "Received packet in ModularTransport" << std::endl;
    if (incomingIpHeader.GetSource() == "10.0.0.2")
    {
        std::cout << "Ack recevied from" << std::endl;
        std::cout << incomingIpHeader.GetSource() << std::endl;
        // #how, function from scheduler?
    }
    else
    {
        std::cout << "Sending back Ack" << std::endl;
        Packet P = Packet();
        // recreate Header for outgoing
        mt->SendPacket(&P, incomingIpHeader.GetDestination(), incomingIpHeader.GetSource());
    }
    std::cout << "################################" << std::endl;
    ;
    return IpL4Protocol::RX_OK;
}
} // namespace ns3
