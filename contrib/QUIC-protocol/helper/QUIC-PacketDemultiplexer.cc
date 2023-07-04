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

    demultiplexePacket();

    //update the context if needed
    QUICContext* newContext = static_cast<QUICContext*>(epOut->context);

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
    // process recvPacket, fill FrameToStream
    // TODO: this is temporary, need to change to approriate headers

    // az:::: HEADER: 1-hello_HEADER: 2-world_HEADER: 1-goood_HEADER: 2-bye_
    // we need to fill them to frameTOStream 
    // structure => std::vector<std::pair<uint64_t, QUICFrame>>
    //  QUICFrame ->
    // worry about the uint64_t later

    // get the actual data within the recvPacket
    Ptr<Packet> copy = recvPacket->Copy();
    uint8_t buffer[copy->GetSize()];
    copy->CopyData(buffer, copy->GetSize());
    
    // loop through buffer can create frame for each QUICFrame
    std::vector<std::string> receivedDataVector;
    std::string frameData;
    for( auto b : buffer ) {
        if (b == '_' ){
            receivedDataVector.push_back( frameData );
            frameData = "";
        } else {
            frameData += b;
        }
    }

    for( unsigned i = 0; i < receivedDataVector.size(); i++ ){
        std::string currentFrameData = receivedDataVector[ i ];
        std::cout << currentFrameData << std::endl;
        int start = currentFrameData.find("HEADER: ") + 8;
        int end = currentFrameData.find("-");

        int streamID = std::stoi(currentFrameData.substr(start, end - start));
        std::string currentData = currentFrameData.substr(end + 1);
        
        // create the QUICFrame as test send in scheduler
        std::cout << streamID << std::endl;
        std::cout << currentData << std::endl;

        QUICFrame currentFrame = QUICFrame{};

        StreamFrameFields * streamFrameFields = new StreamFrameFields;
        streamFrameFields->StreamID = streamID;

        currentFrame.type = FrameType::STREAM;
        currentFrame.fields = streamFrameFields;

        Ptr<Packet> data = Create<Packet>(reinterpret_cast<const uint8_t*>(currentFrameData.data()), currentFrameData.size());
        currentFrame.data = data;


        std::pair<uint64_t, QUICFrame> currentPair = std::make_pair<uint64_t, QUICFrame>( 1, std::move(currentFrame));
        FrameToStream.push_back( currentPair );
        // FrameToStream.push_back( *currentFrame );
    }
    // process receivedData and push back to frameToStream

    return;
}
} // namespace ns3