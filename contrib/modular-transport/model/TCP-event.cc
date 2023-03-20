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
AckEvent::AckEvent(int flow_id,  uint32_t acknum){
    this->seq=acknum;
    this->flow_id=flow_id;
    this->Type=TCPEventType::ACK_DATA;
}
TimeExpire::TimeExpire(){

}
TimeExpire::TimeExpire(int flow_id, uint32_t seq, double EndTime){
    this->EndTime = EndTime;
    this->seq = seq;
    this->flow_id = flow_id;
    this->Type = TCPEventType::TimeExpire;
}
}
