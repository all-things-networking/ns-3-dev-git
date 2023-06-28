#ifndef MT_DISPATCHER_H
#define MT_DISPATCHER_H
namespace ns3
{
class MTEvent;
class MTEventProcessor;
class MTDispatcher{
    public:
    virtual std::vector<MTEventProcessor*> dispatch(MTEvent*)=0; //map chain of events to processor
};
}
#endif
