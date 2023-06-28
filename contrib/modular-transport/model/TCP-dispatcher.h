#ifndef TCP_DISPATCHER_H
#define TCP_DISPATCHER_H
#include "mt-dispatcher.h"
#include <vector>

namespace ns3
{
class MTEvent;
class MTEventProcessor;
class MTDispatcher;
class TCPDispatcher:public MTDispatcher{
    public:
        TCPDispatcher();
        std::vector<MTEventProcessor*> dispatch(MTEvent*);
};
}
#endif
