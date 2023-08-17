#include "QUIC-ReceiveLogic.h"

#include "ns3/modular-transport.h"
#include "ns3/mt-receiver.h"

#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/QUIC-Header.h"
#include "ns3/QUIC-Frame.h"

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
    // MTHeader recievedHeader;
    // packet->RemoveHeader(recievedHeader);
    // uint8_t* buffer = new uint8_t[packet->GetSize()];
    // int size = packet->CopyData(buffer, packet->GetSize());

    //
    MTQUICShortHeader shortHeader;
    packet->RemoveHeader( shortHeader );
    std::cout << "Packet Number:" << shortHeader.pckNum << std::endl;

    while( packet->GetSize() > 0 ){
        QUICFrameHeader frameHeader;
        packet->RemoveHeader( frameHeader );
        if ( frameHeader.frameType == FrameType::ACK ){
            std::cout << "--- Received ACK frame ---" << std::endl;
            
            std::cout << "    largestACK: " << frameHeader.largestACKed << std::endl;
            std::cout << "    ackRangeCount: " << frameHeader.ackRangeCount << std::endl;
            std::cout << "    firstACKRange: " << frameHeader.firstACKRange << std::endl;
            for(int i = 0; i < frameHeader.ackRangeCount; i++ ){
                uint32_t gap = frameHeader.ACKRanges[ i ].first;
                uint32_t range = frameHeader.ACKRanges[ i ].second;
                std::cout << "    ackRange Gap: " << gap << ", range: " << range << std::endl;
            }

        } else if (frameHeader.frameType == FrameType::MAX_DATA) {
            std::cout << "--- Received MAX_DATA frame ---" << std::endl;
            std::cout << "    maxData: " << frameHeader.maxData << std::endl;
        } else if (frameHeader.frameType == FrameType::MAX_STREAM_DATA) {
            std::cout << "--- Received MAX_STREAM_DATA frame ---" << std::endl;
            std::cout << "    stream ID: " << frameHeader.streamID << std::endl;
            std::cout << "    maxStreamData: " << frameHeader.maxStreamData << std::endl;
        }else {
            std::cout << "--- received other types ---" << std::endl;
        }
    }
    
    // chosenScheduler.OpsAfterRecieved(recievedHeader);
    // chosenScheduler.GenerateEventOnReceive(recievedHeader);
    // recievedHeader.OpsAfterRecieved(); //THis one returns a event
    // std::cout << mt << packet << incomingIpHeader << incomingInterface << std::endl;

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
        // std::cout << "Sending back Ack" << std::endl;
        // Packet P = Packet();
        // // recreate Header for outgoing
        // mt->SendPacket(&P, incomingIpHeader.GetDestination(), incomingIpHeader.GetSource());
    }
    std::cout << "################################" << std::endl;
    ;
    return IpL4Protocol::RX_OK;
}
} // namespace ns3
