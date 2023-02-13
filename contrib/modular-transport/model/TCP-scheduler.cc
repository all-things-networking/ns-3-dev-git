#include "TCP-scheduler.h"
#include "mt-event.h"
namespace ns3{
TCPscheduler::TCPscheduler(){

}
MTEvent TCPscheduler::CreateSendEvent(int flow_id, long time){
    MTEvent send = SendEvent(flow_id, time);
    return send;
}
void TCPscheduler::AddEvent(MTEvent newEvent){
    this->myqueue.emplace_back(newEvent);
}
}
