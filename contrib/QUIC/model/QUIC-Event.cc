#include "QUIC-Event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"

namespace ns3{

SendEvent::SendEvent(){

}
SendEvent::SendEvent(long time, int flow_id){
    this->time=time;
    this->flow_id=flow_id;
}
AckEvent::AckEvent(){

}
AckEvent::AckEvent(int flow_id, int seq){
    this->seq=seq;
    this->flow_id=flow_id;
}

StreamEvent::StreamEvent(int flow_id, StreamEventType streamEventType, StreamEventData data, int stream_id){
    this->streamEventType=streamEventType;
    this->stream_id=stream_id;
    this->flow_id=flow_id;
    this->type = EventType::STREAM_EVENT;
    this->data = data;
}

ResponseEvent::ResponseEvent(int flow_id, ResponseEventType responseEventType, ResponseEventData data, int stream_id){
    this->responseEventType=responseEventType;
    this->stream_id=stream_id;
    this->flow_id=flow_id;
    this->type = EventType::RESPONSE_EVENT;
    this->data = data;
}

ReceivePacketEvent::ReceivePacketEvent(long time, int flow_id, Packet* rcv){
    this->flow_id = flow_id;
    this->time = time;
    this->receivered = rcv;
}
}