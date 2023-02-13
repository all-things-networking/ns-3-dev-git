#ifndef MT_EVENT_H
#define MT_EVENT_H
namespace ns3{
class MTHeader;
class MTEvent{
    public:
    long time;
    int flow_id;
    MTEvent(long time, int flow_id);
};

class SendEvent: public MTEvent{
    public:
    SendEvent(long time, int flow_id);
};
}
#endif
