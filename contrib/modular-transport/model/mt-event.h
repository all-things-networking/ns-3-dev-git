#ifndef MT_EVENT_H
#define MT_EVENT_H
namespace ns3{

enum EventType {
    EMPTY,
    STREAM_EVENT,
    RESPONSE_EVENT
};

class MTHeader;
class MTEvent{
    public:
    long time;
    int flow_id;
    EventType type = EventType::EMPTY;
    MTEvent();
    virtual ~MTEvent();
    MTEvent(long time, int flow_id, EventType type = EventType::EMPTY);
};

}
#endif
