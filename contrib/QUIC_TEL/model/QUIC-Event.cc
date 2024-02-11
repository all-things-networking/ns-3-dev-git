#include "QUIC-Event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/QUIC-Frame.h"
#include "ns3/QUIC-PacketBuffer.h"
#include "ns3/QUIC-Header.h"

namespace ns3{

SendEvent::SendEvent(){

}
SendEvent::SendEvent(long time, int flow_id){
    this->time=time;
    this->flow_id=flow_id;
}
AckEvent::AckEvent(){

}
AckEvent::AckEvent(int flow_id, int seq){
    this->seq=seq;
    this->flow_id=flow_id;
}

StreamEvent::StreamEvent(int flow_id, StreamEventType streamEventType, StreamEventData data, int stream_id){
    this->streamEventType=streamEventType;
    this->stream_id=stream_id;
    this->flow_id=flow_id;
    this->type = EventType::STREAM_EVENT;
    this->data = data;
}

ResponseEvent::ResponseEvent(int flow_id, ResponseEventType responseEventType, ResponseEventData data, int stream_id){
    this->responseEventType=responseEventType;
    this->stream_id=stream_id;
    this->flow_id=flow_id;
    this->type = EventType::RESPONSE_EVENT;
    this->data = data;
}

ReceivePacketEvent::ReceivePacketEvent(long time, int flow_id, Packet* rcv){
    this->flow_id = flow_id;
    this->time = time;
    this->type = EventType::RECEIVEPKT_EVENT;
    this->receivered = rcv;
}

MTEvent* SenderEventCreator::CreateSendEvent(int flow_id, long time){
    // Create random data for now
    StreamEventData data;
    data.text = "hello";

    // TODO: need to free this memory after?
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, 5); // pick random stream_id for now
    return streamEvent;
}

MTEvent* SenderEventCreator::CreateSendPacketEvent(int flow_id, long time){
    StreamEventData data;
    data.text = "";

    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::SEND_PACKET, data, -1);
    return streamEvent;
}

MTEvent* SenderEventCreator::CreateAddDataEvent(int flow_id, long time, std::string text, int stream){
    StreamEventData data;
    data.text = text;

    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, stream);
    return streamEvent;
}

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


Packet* ReceiverEventCreator::CreateFakePacket(std::vector<std::string>& data, int packetNumber, bool setFinBit, int streamOffset, int streamContentOffset ){
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
    while ( true ){
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
        std::cout << currentFrameData << " " << currentStreamNumber << " " << offset[ currentIdx ] << std::endl;
        QUICFrameHeader currentFrameHeader = QUICFrameHeader(currentStreamNumber, offset[ currentIdx ], currentFrameData.size(), FrameType::STREAM, finBit,
            0, 0, 0);
        
        offset[ currentIdx ] += currentFrameData.size();

        Ptr<Packet> currentFrameDataPacket = Create<Packet>(reinterpret_cast<const uint8_t*>(currentFrameData.data()), currentFrameData.size());
        currentFrame->AddHeader( currentFrameHeader );
        currentFrame->AddAtEnd( currentFrameDataPacket );
        QUICPacket->AddAtEnd( currentFrame );
        i++;
    }

    std::cout << "Creating Fake Receiving Packet, packet number: " << quicHeader.pckNum << std::endl;
    // Ptr<Packet> ptrPacket = PacketBuffer->CreatePacket();
    Packet* pkt = GetPointer( QUICPacket );
    return pkt;
}

QUICEvent::QUICEvent(){

}

QUICEvent::QUICEvent(long time, int flow_id, EventType type){
    this->time=time;
    this->flow_id=flow_id;
    this->type = type;
}


}