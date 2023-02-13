#ifndef MT_DISPATCHER_H
#define MT_DISPATCHER_H
namespace ns3
{
class MTEvent;
class MTEventProcessor;

class MTDispatcher{
    virtual MTEventProcessor* dispatch(MTEvent);//map event to processor
};

class TCPDispatcher:public MTDispatcher{
    public:
    TCPDispatcher();
    MTEventProcessor* dispatch(MTEvent);
};

}
#endif
