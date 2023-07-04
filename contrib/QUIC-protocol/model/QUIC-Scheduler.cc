#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/mt-event.h"

#include "QUIC-Scheduler.h"
#include "QUIC-Event.h"

#include <string>

namespace ns3{
QUICScheduler::QUICScheduler(){
}

void QUICScheduler::AddEvent(MTEvent* newEvent){
    myqueue.push(newEvent);
}

MTEvent* QUICScheduler::GetNextEvent(){
    MTEvent* next = this->myqueue.front();
    this->myqueue.pop();
    return next;
}

bool QUICScheduler::isEmpty(){
    return this->myqueue.empty();
}

}