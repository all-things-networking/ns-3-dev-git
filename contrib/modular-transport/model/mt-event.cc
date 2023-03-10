#include "mt-event.h"
namespace ns3{
MTEvent::MTEvent(){

}

MTEvent::~MTEvent(){
}

MTEvent::MTEvent(long time, int flow_id, EventType type){
    this->time=time;
    this->flow_id=flow_id;
    this->type = type;
}

}
