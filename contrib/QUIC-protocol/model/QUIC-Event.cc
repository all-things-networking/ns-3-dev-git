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

// <<<<<<< HEAD
Packet* ReceiverEventCreator::CreateFakePacket(){

    QUICPacketBuffer* PacketBuffer = new QUICPacketBuffer;

    std::vector<std::string> data = { "hello", "goood", "world", "bye" };
    std::vector<int> offset = { 1, 1, 2, 2 };
    int MAX_STREAM_DATA = 5;

    Ptr<Packet> QUICPacket = Create<Packet>();

    for(unsigned i = 0; i < 4; i++){
        std::string currentData = data[i];

        //////////////////// Create a frame of size MAX_STREAM_DATA ////////////////////
        QUICFrame* currentFrame = new QUICFrame;
        // see if it is possible to add the currentData from this line

        // Headers for the frame
        QUICFrameHeader currentFrameHeader = QUICFrameHeader(i % 2 + 1, offset[ i ], currentData.size());
        

        // dataFrame->type = FrameType::STREAM;
        // dataFrame->fields = streamFrameFields;

        // Current stream has some data in databuffer, we should create a frame 
        // Note that Packet is used as a frame as well
        // create the data
        Ptr<Packet> data = Create<Packet>(reinterpret_cast<const uint8_t*>(currentData.data()), currentData.size());

        // Ptr<Packet> newQUICFrame = Create<Packet>();
        
        currentFrame->AddHeader( currentFrameHeader );
        currentFrame->AddAtEnd( data );
        // dataFrame->data = data;
        /////////////////////////////////////////////////////////////////////////////////
        // PacketBuffer->AddFrame(dataFrame);
        QUICPacket->AddAtEnd( currentFrame );
    }

    std::cout << "Creating Fake Receiving Packet" << std::endl;
    // Ptr<Packet> ptrPacket = PacketBuffer->CreatePacket();
    Packet* pkt = GetPointer( QUICPacket );
    return pkt;
}
// =======
// QUICEvent::QUICEvent(){

// }

// QUICEvent::QUICEvent(long time, int flow_id, EventType type){
//     this->time=time;
//     this->flow_id=flow_id;
//     this->type = type;
// }


// >>>>>>> origin/Kevin-Bian
// }