#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-PacketDemultiplexer.h"
#include "QUIC-IntermediateOutput.h"
#include "QUIC-Header.h"

#include "../model/QUIC-Event.h"
#include "../model/QUIC-Context.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max
#include <iostream>

namespace ns3
{

class Packet;
class ModularTransport;


QUICPacketDemultiplexer::QUICPacketDemultiplexer():
MTEventProcessor()
{}
bool
QUICPacketDemultiplexer::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICPacketDemultiplexer::Process(MTEvent* e, EventProcessorOutput* epOut){
    ReceivePacketEvent* rpe = static_cast<ReceivePacketEvent*>(e);
    recvPacket = rpe->receivered;
    std::cout << recvPacket->GetSize() << std::endl;


    // probably we can get the packet number here by reading the header first
    MTQUICShortHeader QUICHeader;
    recvPacket->RemoveHeader(QUICHeader);
    // since we get the packet number now, we can update the receivedPackets in context

    //update the context if needed
    QUICContext* newContext = static_cast<QUICContext*>(epOut->context);

    updateContextReceivePackets(QUICHeader, newContext);
    demultiplexePacket();

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent*> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    std::vector<Packet> packetTobeSend;

    QUICIntermediateOutput* intermOutput = new QUICIntermediateOutput;
    intermOutput->PacketDemultiplexerOut = FrameToStream;

    std::cout << "FrameToStream Size: " << FrameToStream.size() << std::endl;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context=newContext;
        Output->packetToSend=packetTobeSend;
        Output->intermOutput = intermOutput;
    return Output;
}

void QUICPacketDemultiplexer::demultiplexePacket() {
    while (recvPacket->GetSize() > 0 ){ 
        QUICFrameHeader header;
        recvPacket->RemoveHeader(header);
        
        uint8_t buffer[header.length]; // length becomes unknown if we use sub classes
        recvPacket->CopyData(buffer, header.length);

        // TODO: for ACK Frame we probably don't want to deal with the following
        std::string currentData;
        for (auto a : buffer ){
            currentData += a;
        }
        ///////// Testing ///////////
        // std::cout << currentData << std::endl;
        ///////// Testing End //////
        recvPacket->RemoveAtStart( header.length );


        std::pair<QUICFrameHeader, std::string> currentPair = std::make_pair<QUICFrameHeader, std::string>( std::move(header), std::move(currentData) );
        
        // add to FrameToStream
        FrameToStream.push_back( currentPair );
    }
    return;
}

void QUICPacketDemultiplexer::updateContextReceivePackets(MTQUICShortHeader& QUICHeader, QUICContext* newContext){
    
    std::cout << "QUICPacketDemultiplexer Processing Packet number: " << QUICHeader.pckNum << std::endl;
    int currentPacketNumber = QUICHeader.pckNum;

    std::vector<std::pair<int, int>>& receivedPackets = newContext->receivedPackets;
    int receivedPacketsLength = receivedPackets.size();
    if ( receivedPacketsLength == 0 
        || receivedPackets[ receivedPacketsLength - 1 ].second + 1 != currentPacketNumber ){
        
        receivedPackets.push_back( std::make_pair(currentPacketNumber, currentPacketNumber) );
    
    } else {
        int start = receivedPackets[ receivedPacketsLength - 1 ].first;
        receivedPackets[ receivedPacketsLength - 1 ] = std::make_pair( start, currentPacketNumber );
    }
}
} // namespace ns3