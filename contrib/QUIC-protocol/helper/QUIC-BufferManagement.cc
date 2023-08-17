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
#include <unordered_set>

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
    // az: this is a chained function, so we care about epOut for now
    std::cout << "QUICBufferManagement::Process start" << std::endl;

    QUICIntermediateOutput* intermOutput = static_cast<QUICIntermediateOutput*>(epOut->intermOutput);
    QUICContext* qc = static_cast<QUICContext*>(epOut->context);
    
    std::unordered_set<int> updatedStreamIDs;

    for( unsigned int i = 0; i < intermOutput->PacketDemultiplexerOut.size(); i++ ){
        QUICFrameHeader header = intermOutput->PacketDemultiplexerOut[i].first;

        uint32_t streamID = header.streamID;
        uint32_t offset = header.offset;
        bool finBit = header.fin;

        std::string currentData = intermOutput->PacketDemultiplexerOut[i].second;
        
        if ( header.frameType == FrameType::STREAM ){
            intermOutput->ackEliciting = true;
            // check if stream exists. If not, create one according to RFC 9000
            if ( qc->receiverBuffer.find( streamID ) == qc->receiverBuffer.end() ){
                QUICStream* stream = new QUICStream( streamID );
                qc->receiverBuffer[ streamID ] = stream;
                std::cout << "Created new stream with ID: "
                        << qc->receiverBuffer.find(streamID)->second->id << std::endl;
            }

            QUICStream* currentStream = qc->receiverBuffer[ streamID ];
            currentStream->AddToDataBuffer( currentData, offset );
            currentStream->SetFin( finBit );
            // if fin bit is not set, also send a maxStreamData Frame
            //  and maxData Frame
            // update streamDataSize
            if ( currentStream->fin == false ){
                if (currentStream->offset > qc->streamDataSize[ streamID ]) {
                    qc->streamDataSize[ streamID ] = currentStream->offset;
                }
                std::cout << qc->streamDataSize[ streamID ] << std::endl;
                updatedStreamIDs.insert( streamID );
            } else {
                updatedStreamIDs.erase( streamID );
                // std::cout << "Remove stream id: " << streamID << std::endl;
            }
        }
    }

    std::vector<Packet> packetTobeSend;
    generateFlowControlFrames(qc, updatedStreamIDs, intermOutput);

    /////////////////////////// TESTING ReceiverBuffer START ///////////////////////////////////////////////////////
    for( unsigned long i = 0; i < qc->receiverBuffer.size(); i++ ){
        if ( qc->receiverBuffer[ i + 1 ] == nullptr ) continue;
        std::cout << "Testing receiverBuffer " << i + 1 << " ";
        std::cout << qc->receiverBuffer[ i + 1 ]->databuffer << std::endl;
        std::cout << "Testing receiverBuffer " << i + 1 << " ";
        for(auto a: qc->receiverBuffer[ i + 1 ]->is_received ) std::cout << a;
        std::cout << std::endl;
    }
    /////////////////////////// TESTING ReceiverBuffer END ///////////////////////////////////////////////////////
    // check the buffer for each stream, and the whole connection


    std::vector<MTEvent*> newEvents;

    // worry about newEvent and packetTobeSend later
    // no packet from buffer management is created, it will be handled in ACK
    std::cout << "packetToBeSend size: " << packetTobeSend.size() << std::endl;    
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context = qc;
        Output->packetToSend=packetTobeSend;
        Output->intermOutput = intermOutput;
    return Output;
}

void QUICBufferManagement::generateFlowControlFrames( 
    QUICContext* qc, 
    std::unordered_set<int>& updatedStreamIDs,
    QUICIntermediateOutput* intermOutput) {
    // generate MAX_DATA frame
    int totalData = 0;
    for(auto& dataSize : qc->streamDataSize ) totalData += dataSize.second;
    qc->currentTotalDataSize = totalData;
    std::cout << "current total stream data: " << totalData << std::endl;
    std::cout << "updatedStreamIDs size: " << updatedStreamIDs.size() << std::endl;

    int updatedStreamCount = updatedStreamIDs.size();
    uint32_t newMaxData = updatedStreamCount * 20 + totalData;
    if ( newMaxData >= qc->maxData ){
        std::cout << "Cannot receive more data for the connection..." << std::endl;
    } else {
        // create maxData Frame and append to the quicPacket
        QUICFrame* maxDataFrame = new QUICFrame;
        QUICFrameHeader frameHeader = QUICFrameHeader();
        frameHeader.setMaxDataFrameHeader( qc->maxData );
        maxDataFrame->AddHeader( frameHeader );

        intermOutput->QUICFrameBuffer.push_back( maxDataFrame );
    }


    // generate MAX_STREAM_DATA frame
    for( auto& streamID : updatedStreamIDs ){
        QUICFrame* maxStreamDataFrame = new QUICFrame;
        
        QUICFrameHeader frameHeader = QUICFrameHeader();
        uint32_t maxFrameData = qc->streamDataSize[ streamID ] + 10;
        frameHeader.setMaxStreamFrameHeader( streamID, maxFrameData );
        maxStreamDataFrame->AddHeader( frameHeader );

        intermOutput->QUICFrameBuffer.push_back( maxStreamDataFrame );
    }
}
} // namespace ns3