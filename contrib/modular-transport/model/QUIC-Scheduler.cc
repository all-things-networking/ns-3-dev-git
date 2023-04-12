#include "QUIC-Scheduler.h"
#include "mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include <string>
namespace ns3{
    
QUICScheduler::QUICScheduler(){

}

// Temporary: We are overriding this to create a stream event! 
// TODO: currently we allow the user to specify the type of stream event and the id
// in the future, we can abstract this from the user
MTEvent* QUICScheduler::CreateSendEvent(int flow_id, long time){
    // Create random data for now
    std::string data = "hello";

    // TODO: need to free this memory after?
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, 5); // pick random stream_id for now
    return streamEvent;
}

MTEvent* QUICScheduler::SendPacket(int flow_id, long time){
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::SEND_PACKET, "", -1); // pick random stream_id for now
    return streamEvent;
}

MTEvent* QUICScheduler::AddData(int flow_id, long time, std::string data, int stream){
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, stream);
    return streamEvent;
}

void QUICScheduler::AddEvent(MTEvent* newEvent){
    this->myqueue.push(newEvent);
}

bool QUICScheduler::isEmpty(){
    return this->myqueue.empty();
}

MTEvent* QUICScheduler::GetNextEvent(){
    MTEvent* next = this->myqueue.front();
    this->myqueue.pop();
    return next;
}
}
