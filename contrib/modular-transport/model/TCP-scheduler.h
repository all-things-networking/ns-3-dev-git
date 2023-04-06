#ifndef MT_TCP_SCHEDULER_H
#define MT_TCP_SCHEDULER_H
#include "mt-scheduler.h"
#include <queue>
namespace ns3{
class MTScheduler;
class MTEvent;
class MTHeader;
class ModularTransport;
class TCPScheduler: public MTScheduler{
    public:
    TCPScheduler();
    MTEvent* GetNextEvent();
    void enqueue(MTEvent*);
    MTEvent* CreateSendEvent(int, long);
    MTEvent* CreateAckEvent(int, int);
    bool isEmpty();
    private:
        //TODO: seperate queues for different types of events
        std::queue<MTEvent*> myqueue;
    };
}

#endif
