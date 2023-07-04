#ifndef MT_EVENT_H
#define MT_EVENT_H
namespace ns3{

class MTHeader;
class MTEvent{
    public:
    long time;
    int flow_id;
    MTEvent();
    virtual ~MTEvent();
};

}
#endif
