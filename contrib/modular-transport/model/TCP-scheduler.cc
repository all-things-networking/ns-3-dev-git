#include "TCP-scheduler.h"
#include "mt-event.h"
#include "TCP-event.h"
#include "modular-transport.h"
namespace ns3{
TCPScheduler::TCPScheduler(){

}
MTEvent* TCPScheduler::CreateSendEvent(int flow_id, long time){
    //#Add type here?
    MTEvent* send = new SendEvent(flow_id, time);
    return send;
}
MTEvent* TCPScheduler::CreateAckEvent(int flow_id, int seq){
    MTEvent* ack = new  AckEvent(flow_id, seq);
    return ack;
}
void TCPScheduler::enqueue(MTEvent* newEvent){
    this->myqueue.push(newEvent);
}
void TCPScheduler::AddEvent(MTEvent* newEvent, ModularTransport* mt){
    if(this->myqueue.empty()){
                this->enqueue(newEvent);
                mt->Mainloop();
    }else{
                this->enqueue(newEvent);
    }
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
