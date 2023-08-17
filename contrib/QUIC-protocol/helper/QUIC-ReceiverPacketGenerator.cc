#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-ReceiverPacketGenerator.h"
#include "QUIC-IntermediateOutput.h"
#include "QUIC-Header.h"

#include "../model/QUIC-Event.h"
#include "../model/QUIC-Context.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max
#include <iostream>
#include <unordered_set>

namespace ns3
{

class Packet;
class ModularTransport;


QUICReceiverPacketGenerator::QUICReceiverPacketGenerator():
MTEventProcessor()
{}
bool
QUICReceiverPacketGenerator::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICReceiverPacketGenerator::Process(MTEvent* e, EventProcessorOutput* epOut){
    std::cout << "QUICReceiverPacketGenerator::Process start" << std::endl;

    QUICIntermediateOutput* intermOutput = static_cast<QUICIntermediateOutput*>(epOut->intermOutput);
    QUICContext* qc = static_cast<QUICContext*>(epOut->context);
    
    std::vector<Packet> packetTobeSend;
    std::vector<MTEvent*> newEvents;

    int frameCount = 0;
    Ptr<Packet> QUICPacket = Create<Packet>();

    MTQUICShortHeader quicHeader = MTQUICShortHeader();
    // TODO: TFC 12.3, each endpoint maintain a separate packenumber for sending and receiving
    quicHeader.pckNum = qc->receiverPacketNumber;
    qc->receiverPacketNumber += 1;
    QUICPacket->AddHeader( quicHeader );
    
    for( unsigned i = 0; i < intermOutput->QUICFrameBuffer.size(); i++ ){
        if ( frameCount == qc->frameLimit ){
          frameCount = 0;
          Packet* pkt = GetPointer( QUICPacket );
          packetTobeSend.push_back( *pkt );

          
          QUICPacket = Create<Packet>();
          quicHeader = MTQUICShortHeader();
          quicHeader.pckNum = qc->receiverPacketNumber;
          qc->receiverPacketNumber++;
          QUICPacket->AddHeader( quicHeader ); 
        }
        QUICFrame* currentFrame = intermOutput->QUICFrameBuffer[ i ];
        QUICPacket->AddAtEnd( currentFrame );
        frameCount++;
    }

    if ( frameCount > 0 ){
      Packet* pkt = GetPointer( QUICPacket );
      packetTobeSend.push_back( *pkt );
    }


    std::cout << "packetTobeSend size: " << packetTobeSend.size() << std::endl;    
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context = qc;
        Output->packetToSend=packetTobeSend;
    return Output;
}
} // namespace ns3