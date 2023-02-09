#include "TCP-schedular"
#include "mt-event"
namespace ns3{
MTEvent TCPscheduler::CreateSendEvent(int flow_id, long time){
    MTEvent send = SendEvent(flow_id, time);
    return send;
}
}
