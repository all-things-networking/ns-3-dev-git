#include "mt-event.h"
#include "QUIC-Event.h"
namespace ns3{
SendEvent::SendEvent(){

}
SendEvent::SendEvent(long time, int flow_id){
    this->time=time;
    this->flow_id=flow_id;
}
}
