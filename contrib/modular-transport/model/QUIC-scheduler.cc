#include "QUIC-scheduler.h"
#include "mt-event.h"
#include "QUIC-event.h"


namespace ns3{
QUICScheduler::QUICScheduler(){

}

// Temporary: We are overriding this to create a stream event! 
// TODO: currently we allow the user to specify the type of stream event and the id
// in the future, we can abstract this from the user
MTEvent* QUICScheduler::CreateSendEvent(int flow_id, long time){

}

MTEvent* QUICScheduler::CreateReceiveEvent(int flow_id, long time, MTPacket* pkg){
    MTEvent* RCVPacketEvent = new ReceivePacketEvent(time, flow_id, pkg); 
    return RCVPacketEvent;
}

void QUICScheduler::AddEvent(MTEvent* newEvent){
    myqueue.push(newEvent);
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