#ifndef QUIC_DISPATCHER_H
#define QUIC_DISPATCHER_H
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

    // Dispatches the appropriate event processor based on the event type
    std::vector<MTEventProcessor*> dispatch(MTEvent*);
};

}
#endif