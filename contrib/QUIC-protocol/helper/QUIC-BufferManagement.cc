#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-BufferManagement.h"
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


QUICBufferManagement::QUICBufferManagement():
MTEventProcessor()
{}
bool
QUICBufferManagement::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICBufferManagement::Process(MTEvent* e, EventProcessorOutput* epOut){
    // seperate queues for each packet
    // size for each queue
    // what to send back

    // az: this is a chained function, so we care about epOut for now
    std::cout << "QUICBufferManagement::Process start" << std::endl;

    // QUICIntermediateOutput intermOutput = static_cast<QUICIntermediateOutput>(epOut->intermOutput);
    QUICIntermediateOutput* intermOutput = static_cast<QUICIntermediateOutput*>(epOut->intermOutput);
    QUICContext* qc = static_cast<QUICContext*>(epOut->context);
    
    for( unsigned int i = 0; i < intermOutput->PacketDemultiplexerOut.size(); i++ ){
        QUICFrameHeader header = intermOutput->PacketDemultiplexerOut[i].first;
        std::cout << header.streamID << " " << header.offset << " " << header.length << std::endl;
        // QUICFrame currentFrame = intermOutput->PacketDemultiplexerOut[i].second;

        // Ptr<Packet> copy = currentFrame.data->Copy();
        // uint8_t buffer[copy->GetSize()];
        // copy->CopyData(buffer, copy->GetSize());

        // std::string currentFrameData;
        // for (auto c : buffer ) currentFrameData += c;

        // int start = currentFrameData.find("HEADER: ") + 8;
        // int end = currentFrameData.find("-");

        // int streamID = std::stoi(currentFrameData.substr(start, end - start));
        // std::string currentData = currentFrameData.substr(end + 1);
        
        // // it shouldn't be like this as the stream should be set up
        // // when openning the connection
        // // look at RFC 9000 ******
        // if ( qc->receiverBuffer.find( streamID ) == qc->receiverBuffer.end() ){
        //     // create an entry in the receiverBuffer
        //     QUICStream* stream = new QUICStream( streamID );
        //     qc->receiverBuffer[ streamID ] = stream;
        //     std::cout << "Created new stream with ID: "
        //               << qc->receiverBuffer.find(streamID)->second->id << std::endl;
        // }

        // qc->receiverBuffer[ streamID ]->AddToDataBuffer( currentData );
    }

    std::cout << qc->receiverBuffer[ 1 ]->databuffer << std::endl;
    std::cout << qc->receiverBuffer[ 2 ]->databuffer << std::endl;
    // check the buffer for each stream, and the whole connection

    //update the context if needed
    // MTContext newContext = *(epOut->context);

    std::vector<MTEvent*> newEvents;

    // worry about newEvent and packetTobeSend later
    // no packet from buffer management is created, it will be handled in ACK
    std::vector<Packet> packetTobeSend;
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context = qc;
        Output->packetToSend=packetTobeSend;
    return Output;
}
} // namespace ns3