#include "TCP-scheduler.h"
#include "mt-event.h"
namespace ns3{
MTEvent TCPscheduler::CreateSendEvent(int flow_id, long time){
    MTEvent send = SendEvent(flow_id, time);
    return send;
}
}
