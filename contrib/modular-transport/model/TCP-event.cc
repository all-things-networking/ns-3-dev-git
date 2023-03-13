#include "mt-event.h"
#include "TCP-event.h"
namespace ns3{
SendEvent::SendEvent(){

}
SendEvent::SendEvent(long time, int flow_id){
    this->time=time;
    this->flow_id=flow_id;
    this->Type=TCPEventType::SEND_DATA;
}
AckEvent::AckEvent(){

}
AckEvent::AckEvent(int flow_id, int seq){
    this->seq=seq;
    this->flow_id=flow_id;
    this->Type=TCPEventType::ACK_DATA;
}
}
