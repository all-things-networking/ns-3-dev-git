#include "QUIC-Event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/QUIC-Frame.h"
#include "ns3/QUIC-PacketBuffer.h"

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

Packet* ReceiverEventCreator::CreateFakePacket(){

    QUICPacketBuffer* PacketBuffer = new QUICPacketBuffer;

    std::vector<std::string> data = { "hello", "goood", "world", "bye" };
    int MAX_STREAM_DATA = 5;

    for(unsigned i = 0; i < 4; i++){
        std::string curr_data = data[i];

        //////////////////// Create a frame of size MAX_STREAM_DATA ////////////////////
        QUICFrame* dataFrame = new QUICFrame;

        // Headers/fields for the frame
        StreamFrameFields * streamFrameFields = new StreamFrameFields;
        streamFrameFields->StreamID = i % 2 + 1;
        std::cout << "stream ID: " << streamFrameFields->StreamID << std::endl;
        
        dataFrame->type = FrameType::STREAM;
        dataFrame->fields = streamFrameFields;

        // The "-" is used to seperate header and "_" is used to seperate frames
        // TODO: this is temporary, need to change to approriate headers
        std::string substrWithHeader = dataFrame->generateHeader() + "-" + curr_data.substr(0, MAX_STREAM_DATA) + "_";

        // Current stream has some data in databuffer, we should create a frame 
        // Note that Packet is used as a frame as well
        Ptr<Packet> data = Create<Packet>(reinterpret_cast<const uint8_t*>(substrWithHeader.data()), substrWithHeader.size());
        dataFrame->data = data;
        /////////////////////////////////////////////////////////////////////////////////
        PacketBuffer->AddFrame(dataFrame);
    }

    std::cout << "Creating Fake Receiving Packet" << std::endl;
    Ptr<Packet> ptrPacket = PacketBuffer->CreatePacket();
    Packet* pkt = GetPointer( ptrPacket );
    std::cout << *ptrPacket << std::endl;
    // std::cout << pkt->GetSize() << std::endl;
    return pkt;
}
}