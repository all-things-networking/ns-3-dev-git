#include "QUIC_TEL-Event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "../helper/QUIC_TEL-Frame.h"
#include "../helper/QUIC_TEL-QuicHeader.h"

namespace ns3{

SendEvent::SendEvent(){

}

SendEvent::SendEvent(int flow_id){
    this->flow_id=flow_id;
}

AckEvent::AckEvent(){
}

AckEvent::AckEvent(int flow_id) {
    this->flow_id=flow_id;
    this->largest = 0; //NOT SURE if these are initialized correctly
    this->ECT0_Count = 0;
    this->ECT1_Count = 0;
    this->ECN_CE_Count = 0;
}

TimerEvent::TimerEvent() {

}

AddDataEvent::AddDataEvent() {

}

AddDataEvent::AddDataEvent(bool server_side, bool direction, std::string data) {
    this->server_side = server_side;
    this->direction = direction;
    this->data = data;
}

CongestionEvent::CongestionEvent(int time_sent) {
    this->time_sent = time_sent;
}

///////////////////////////////////////code for testing///////////////////////////////////////

    
MTEvent* SenderEventCreator::CreateAddDataEvent(bool server_side, bool direction, std::string data) {
    MTEvent* AddingDataEvent = new AddDataEvent(server_side, direction, data); 
    return AddingDataEvent;
}

MTEvent* SenderEventCreator::CreateSendPacketEvent(int flow_id){
    MTEvent* SendPacketEvent = new SendEvent(flow_id);
    return SendPacketEvent;
}


/* respond and receive event is not yet implemented
MTEvent* SenderEventCreator::CreateACKPacketEvent(int flow_id, long time, int packetNum){
        ResponseEventData data;
        data.text = "";
        data.packetNum = packetNum;
        MTEvent* streamEvent = new ResponseEvent(flow_id, ResponseEventType::ACK_PACKET, data, -1);
        return streamEvent;
}

MTEvent* ReceiverEventCreator::CreateReceiveEvent(int flow_id, long time, Packet* pkg){
        MTEvent* RCVPacketEvent = new ReceivePacketEvent(time, flow_id, pkg);
        return RCVPacketEvent;
}


Packet* ReceiverEventCreator::CreateFakePacket(std::vector<std::string>& data, int packetNumber, bool setFinBit, int streamOffset, int streamContentOffset ) {
        // TODO: make this more flexible

        QUICPacketBuffer* PacketBuffer = new QUICPacketBuffer;

        int MAX_STREAM_DATA = 5; // hard coded
        std::vector<int> offset( data.size(), 0 + streamContentOffset );

        Ptr<Packet> QUICPacket = Create<Packet>();

        // create short header
        // QUICPacket should have a short header
        // long header -> used for packets sent prior to establishment of 1-RTT keys
        MTQUICShortHeader quicHeader = MTQUICShortHeader();
        quicHeader.pckNum = packetNumber;
        QUICPacket->AddHeader( quicHeader );

        int totalLength = 0;
        for ( auto& streamData : data ){
            totalLength += streamData.size();
        }

        int processedLength = 0;
        int i = 0;
        while ( true ) {
            if ( processedLength == totalLength ) break;

            int currentIdx = i % data.size();
            std::string& currentData = data[ currentIdx ];

            if ( currentData.size() == 0 ){
                i++;
                continue;
            }

            std::string currentFrameData = currentData.substr( 0, MAX_STREAM_DATA );

            // process input
            currentData = currentData.substr( currentFrameData.length() );
            processedLength += currentFrameData.size();

            int currentStreamNumber = currentIdx + 1 + streamOffset;
            QUICFrame* currentFrame = new QUICFrame;
            int finBit = currentData.size() == 0 && setFinBit ? 1 : 0;

            // std::cout << offset[ currentIdx ] << std::endl;
            std::cout << currentFrameData << " " << currentStreamNumber << " " << offset[currentIdx]
            << std::endl; QUICFrameHeader currentFrameHeader = QUICFrameHeader(currentStreamNumber, offset[currentIdx],
            currentFrameData.size(), FrameType::STREAM, finBit, 0, 0, 0);

            offset[ currentIdx ] += currentFrameData.size();

            Ptr<Packet> currentFrameDataPacket = Create<Packet>(reinterpret_cast<const uint8_t*>(currentFrameData.data()), currentFrameData.size());
            currentFrame->AddHeader(currentFrameHeader);
            currentFrame->AddAtEnd(currentFrameDataPacket);
            QUICPacket->AddAtEnd(currentFrame);
            i++;
        }

        std::cout << "Creating Fake Receiving Packet, packet number: " << quicHeader.pckNum << std::endl;
        // Ptr<Packet> ptrPacket = PacketBuffer->CreatePacket();
        Packet* pkt = GetPointer( QUICPacket );
        return pkt;
    }
*/
/////////////////////////////////////////////////////////////////////////////////

QUICEvent::QUICEvent() {
}

QUICEvent::QUICEvent(long time, int flow_id, EventType type){
    this->time=time;
    this->flow_id=flow_id;
    this->type = type;
}

}