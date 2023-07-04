#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/mt-event.h"

#include "QUIC-Scheduler.h"
#include "QUIC-Event.h"

#include <string>

namespace ns3{
QUICScheduler::QUICScheduler(){

}

// Temporary: We are overriding this to create a stream event! 
// TODO: currently we allow the user to specify the type of stream event and the id
// in the future, we can abstract this from the user
MTEvent* QUICScheduler::CreateSendEvent(int flow_id, long time){
    // Create random data for now
    StreamEventData data;
    data.text = "hello";

    // TODO: need to free this memory after?
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, 5); // pick random stream_id for now
    return streamEvent;
}

// TODO: we need some Packet builder


MTEvent* QUICScheduler::CreateReceiveEvent(int flow_id, long time, Packet* pkt){
    MTEvent* RCVPacketEvent = new ReceivePacketEvent(time, flow_id, pkt); 
    return RCVPacketEvent;
}

void QUICScheduler::AddEvent(MTEvent* newEvent){
    myqueue.push(newEvent);
}

MTEvent* QUICScheduler::GetNextEvent(){
    MTEvent* next = this->myqueue.front();
    this->myqueue.pop();
    return next;
}

MTEvent* QUICScheduler::SendPacket(int flow_id, long time){
    StreamEventData data;
    data.text = "";

    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::SEND_PACKET, data, -1);
    return streamEvent;
}

MTEvent* QUICScheduler::AddData(int flow_id, long time, std::string text, int stream){
    StreamEventData data;
    data.text = text;

    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, stream);
    return streamEvent;
}

MTEvent* QUICScheduler::ACKPacket(int flow_id, long time, int packetNum){
    ResponseEventData data;
    data.text = "";
    data.packetNum = packetNum;

    MTEvent* streamEvent = new ResponseEvent(flow_id, ResponseEventType::ACK_PACKET, data, -1);
    return streamEvent;
}

bool QUICScheduler::isEmpty(){
    return this->myqueue.empty();
}

Packet* QUICScheduler::CreateFakePacket(){
    // re-use the packetBuffer from Context?

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