#include "ns3/mt-state.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

#include "QUIC-ReceiverAckHandler.h"
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


QUICAckHandler::QUICAckHandler():
MTEventProcessor()
{}
bool
QUICAckHandler::IsValidEvent(MTEvent* e)
{
    return true;
}

EventProcessorOutput* QUICAckHandler::Process(MTEvent* e, EventProcessorOutput* epOut){

    std::cout << "QUICAckHandler::Process called" << std::endl;

    QUICContext* qc = static_cast<QUICContext*>(epOut->context);
    QUICIntermediateOutput* intermOutput = static_cast<QUICIntermediateOutput*>(epOut->intermOutput);
    // std::vector<Frame> bufferedFrames = epOut->bufferedFrames;
    // TODO: consider adding a separate event processor at the end for packet creation

    //update the context if needed
    int currentPacketNumber = intermOutput->currentPacketNumber;
    updateContextReceivePackets( currentPacketNumber, qc );
    // for( auto& a : qc->receivedPackets ) std::cout << a.first << " --------------------------------- " << a.second << std::endl;

    std::vector<Packet> packetTobeSend = epOut->packetToSend;


    // create an ack frame and send immediately
    // get stuff from QUIC-Context->receivedPackets to generate the range
    if ( intermOutput->ackEliciting ){
        std::cout << "ACK ELIciting" << std::endl;
        for(long unsigned i = 0; i < qc->receivedPackets.size(); i++ ){
            std::cout << qc->receivedPackets[ i ].first << " " << qc->receivedPackets[ i ].second << std::endl;
        }
        // Use the largest acknowledgement number 
        uint32_t largestRangeIdx = qc->receivedPackets.size() - 1;
        uint32_t largestAck = qc->receivedPackets[ largestRangeIdx ].second;
        uint32_t firstACKRange = largestAck - qc->receivedPackets[ largestRangeIdx ].first;

        uint32_t ackRangeCount = qc->receivedPackets.size() - 1;

        // // create an ACK Frame and send out the result
        Ptr<Packet> QUICPacket = Create<Packet>();

        MTQUICShortHeader quicHeader = MTQUICShortHeader();
        quicHeader.pckNum = largestAck + 20; // needs to be changed
        // std::cout << "ACK packet number: " << quicHeader.pckNum << std::endl;
        QUICPacket->AddHeader( quicHeader );

        QUICFrame* ackFrame = new QUICFrame;


        QUICFrameHeader currentFrameHeader = QUICFrameHeader();
        currentFrameHeader.setACKFrameHeader( largestAck, ackRangeCount, firstACKRange );

        // Example:
        //  packets received are: [(1, 3), (6, 6)]
        //   largsest: 6
        //   firstAckRange: 0
        //   gap: 6 - 1 - 3 = 2
        //   ack range length: 3 - 1 = 2
        for( int i = largestRangeIdx - 1; i >= 0; i-- ){
            uint32_t gap = qc->receivedPackets[ i + 1 ].first - 1 - qc->receivedPackets[ i ].second;
            uint32_t ACKRangeLength = qc->receivedPackets[ i ].second - qc->receivedPackets[ i ].first;
            currentFrameHeader.ACKRangesPush( gap, ACKRangeLength );
        }
        ackFrame->AddHeader( currentFrameHeader ); // header may be enough for ack frame
        // QUICPacket->AddAtEnd( ackFrame );
        intermOutput->QUICFrameBuffer.push_back( ackFrame );
    }

    //add any new events (e.g. application wants to terminate connection)
    std::vector<MTEvent*> newEvents;

    //update packet to be send (e.g. connection termination, state change)
    // packetTobeSend.emplace_back(ackPacket);
    std::cout << "packetToBeSend size: " << packetTobeSend.size() << std::endl;    
    EventProcessorOutput *Output = new EventProcessorOutput;
        Output->newEvents=newEvents;
        Output->context=qc;
        Output->packetToSend=packetTobeSend;
        Output->intermOutput=intermOutput;
    return Output;
}

void QUICAckHandler::updateContextReceivePackets(int currentPacketNumber, QUICContext* newContext){
    
    std::cout << "QUICAckHandler Processing Packet number: " << currentPacketNumber << std::endl;
    std::vector<std::pair<int, int>>& receivedPackets = newContext->receivedPackets;
    int receivedPacketsLength = receivedPackets.size();
    if ( receivedPacketsLength == 0 
        || receivedPackets[ receivedPacketsLength - 1 ].second + 1 < currentPacketNumber ){
        
        receivedPackets.push_back( std::make_pair(currentPacketNumber, currentPacketNumber) );
    
    } else {
        std::vector<std::pair<int, int>> newReceivedPackets;

        int i = 0;
        // [[1,3], [6,6]]
        while( i < receivedPacketsLength ) {
            if ( receivedPackets[ i ].first - 1 == currentPacketNumber ) {
                newReceivedPackets.push_back( std::make_pair(receivedPackets[ i ].first - 1, receivedPackets[ i ].second) );
            } else if ( receivedPackets[ i ].second + 1 == currentPacketNumber ){
                int currentStart = receivedPackets[ i ].first;
                int currentEnd;
                if ( i + 1 < receivedPacketsLength && receivedPackets[ i + 1 ].first - 1 == currentPacketNumber ){
                    currentEnd = receivedPackets[ i + 1 ].second;
                    i++;
                } else {
                    currentEnd = currentPacketNumber;
                }
                newReceivedPackets.push_back( std::make_pair( currentStart, currentEnd ) );
            } else {
                // TODO: [1, 3] [7, 7] adding interval 5,5
                newReceivedPackets.push_back( receivedPackets[ i ] ); 
                if ( receivedPackets[ i ].second < currentPacketNumber &&
                    (i + 1 == receivedPacketsLength || receivedPackets[ i + 1 ].first - 1 > currentPacketNumber) ){
                    newReceivedPackets.push_back( std::make_pair(currentPacketNumber, currentPacketNumber) );
                }
            }
            i++;
        }
        newContext->receivedPackets = newReceivedPackets;
    }
}
} // namespace ns3