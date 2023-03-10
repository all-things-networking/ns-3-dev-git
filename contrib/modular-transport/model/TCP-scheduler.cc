#include "TCP-scheduler.h"
#include "mt-event.h"
#include "TCP-event.h"
namespace ns3{
TCPScheduler::TCPScheduler(){

}
MTEvent* TCPScheduler::CreateSendEvent(int flow_id, long time){
    //#Add type here?
    MTEvent* send = SendEvent(flow_id, time);
    return send;
}
MTEvent* TCPScheduler::CreateAckEvent(int flow_id, int seq){
    MTEvent* ack = AckEvent(flow_id, seq);
    return ack;
}
void TCPScheduler::AddEvent(MTEvent* newEvent){
    this->myqueue.push(newEvent);
}
bool TCPScheduler::isEmpty(){
    return this->myqueue.empty();
}
MTEvent* TCPScheduler::GetNextEvent(){
    MTEvent* next = this->myqueue.front();
    this->myqueue.pop();
    return next;
}
}
