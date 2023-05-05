#ifndef QUICRECEIVER_DISPATCHER_H
#define QUICRECEIVER_DISPATCHER_H
#include "ns3/mt-dispatcher.h"
#include <vector>

namespace ns3
{

class MTEvent;
class MTEventProcessor;
class MTDispatcher;

class QUICDispatcher:public MTDispatcher{
    public:
    QUICDispatcher();
    std::vector<MTEventProcessor*> dispatch(MTEvent*);
};

}
#endif