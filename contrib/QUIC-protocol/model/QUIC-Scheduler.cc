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

MTEvent* QUICScheduler::CreateReceiveEvent(int flow_id, long time, Packet* pkg){
    MTEvent* RCVPacketEvent = new ReceivePacketEvent(time, flow_id, pkg); 
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

}