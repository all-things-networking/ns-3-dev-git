#include "QUIC-Scheduler.h"
#include "mt-event.h"

namespace ns3{
QUICScheduler::QUICScheduler(){

}

// Temporary: We are overriding this to create a stream event! 
// TODO: currently we allow the user to specify the type of stream event and the id
// in the future, we can abstract this from the user
MTEvent* QUICScheduler::CreateSendEvent(int flow_id, long time){
    // TODO: need to free this memory after?
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::SEND_DATA, 5); // pick random stream_id for now
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
