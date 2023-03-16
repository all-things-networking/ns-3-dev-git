#ifndef MT_DISPATCHER_H
#define MT_DISPATCHER_H
#include <vector>
namespace ns3
{
class MTEvent;
class MTEventProcessor;

class MTDispatcher{
    virtual std::vector<MTEventProcessor*> dispatch(MTEvent*)=0;//map event to processor
};

}
#endif
