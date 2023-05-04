#ifndef QUICSENDER_DISPATCHER_H
#define QUICSENDER_DISPATCHER_H
#include "ns3/mt-dispatcher.h"
namespace ns3
{
class MTEvent;
class MTEventProcessor;
class MTDispatcher;
class QUICDispatcher:public MTDispatcher{
    public:
        QUICDispatcher();
        MTEventProcessor* dispatch(MTEvent* event);
};
}
#endif
