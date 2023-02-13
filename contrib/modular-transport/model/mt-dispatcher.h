#ifndef MT_DISPATCHER_H
#define MT_DISPATCHER_H
namespace ns3
{
class MTEvent;
class MtEventProcessor;

class MTDispatcher{
    virtual MtEventProcessor* dispatch(MTEvent);//map event to processor
};

class TCPDispatcher:public MTDispatcher{
    TCPDispatcher();
    MtEventProcessor* dispatch(MTEvent);
};

}
#endif
