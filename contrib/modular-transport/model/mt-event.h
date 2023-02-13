#ifndef MT_EVENT_H
#define MT_EVENT_H
namespace ns3{
class MTHeader;
class MTEvent{
    long time;
    int flow_id;
    MTEvent(long time, int flow_id);
};

class SendEvent: public MTEvent{
    SendEvent(long time, int flow_id);
};
}
#endif
