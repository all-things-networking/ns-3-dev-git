#include "QUIC-Scheduler.h"
#include "mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include <string>
namespace ns3{
    
QUICScheduler::QUICScheduler(){

}

// Temporary: We are overriding this to create a stream event! 
// TODO: currently we allow the user to specify the type of stream event and the id
// in the future, we can abstract this from the user
MTEvent* QUICScheduler::CreateSendEvent(int flow_id, long time){
    // Create random data for now
    Ptr<Packet> data = Create<Packet>(reinterpret_cast<const uint8_t*>("hello"), 5);

    // TODO: need to free this memory after?
    MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data, 5); // pick random stream_id for now
    return streamEvent;
}

std::vector<MTEvent*> SendString(int flow_id, long time, std::string text){
    int MAX_STREAM_DATA = 5; // Temporary for now

    std::vector<MTEvent*> events;

    for (int i = 0; i < text.length(); i += MAX_STREAM_DATA) {
        std::string substr = text.substr(i, MAX_STREAM_DATA);

        // This is not a packet, it is just used to temporarily hold the data
        Ptr<Packet> data = Create<Packet>(reinterpret_cast<const uint8_t*>(substr.data()), MAX_STREAM_DATA);
        MTEvent* streamEvent = new StreamEvent(flow_id, StreamEventType::ADD_DATA, data); // let the event processor create a stream ID

        events.push_back(streamEvent);
    }

    return events;
}

void QUICScheduler::AddEvent(MTEvent* newEvent){
    this->myqueue.push(newEvent);
}

bool QUICScheduler::isEmpty(){
    return this->myqueue.empty();
}

MTEvent* QUICScheduler::GetNextEvent(){
    MTEvent* next = this->myqueue.front();
    this->myqueue.pop();
    return next;
}
}
