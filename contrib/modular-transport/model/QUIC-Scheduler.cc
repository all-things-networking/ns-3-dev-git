#include "QUIC-Scheduler.h"
#include "mt-event.h"
#include "QUIC-Event.h"
namespace ns3{
QUICScheduler::QUICScheduler(){

}
MTEvent QUICScheduler::CreateSendEvent(int flow_id, long time){
    MTEvent send = SendEvent(flow_id, time);
    return send;
}
void QUICScheduler::AddEvent(MTEvent newEvent){
    this->myqueue.push(newEvent);
}
bool QUICScheduler::isEmpty(){
    return this->myqueue.empty();
}
MTEvent QUICScheduler::GetNextEvent(){
    MTEvent next = this->myqueue.front();
    this->myqueue.pop();
    return next;
}
}
