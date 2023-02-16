#include "mt-event.h"
namespace ns3{
MTEvent::MTEvent(){

}
MTEvent::MTEvent(long time, int flow_id){
    this->time=time;
    this->flow_id=flow_id;
}
SendEvent::SendEvent(){

}
SendEvent::SendEvent(long time, int flow_id){
    this->time=time;
    this->flow_id=flow_id;
}
}
