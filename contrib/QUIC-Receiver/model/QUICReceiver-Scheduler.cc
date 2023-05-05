#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/mt-event.h"

#include "QUICReceiver-Scheduler.h"
#include "QUICReceiver-Event.h"

namespace ns3{
QUICScheduler::QUICScheduler(){

}

// Temporary: We are overriding this to create a stream event! 
// TODO: currently we allow the user to specify the type of stream event and the id
// in the future, we can abstract this from the user
MTEvent* QUICScheduler::CreateSendEvent(int flow_id, long time){

}

MTEvent* QUICScheduler::CreateReceiveEvent(int flow_id, long time, Packet* pkg){
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