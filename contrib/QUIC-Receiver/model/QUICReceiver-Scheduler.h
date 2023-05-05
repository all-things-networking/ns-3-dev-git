#ifndef QUICRECEIVER_SCHEDULER_H
#define QUICRECEIVER_SCHEDULER_H
#include "ns3/mt-scheduler.h"

#include "QUICReceiver-Event.h"
#include <queue>
namespace ns3{
class MTScheduler;
class MTEvent;
class MTHeader;
class QUICScheduler: public MTScheduler{
    public:
    std::queue<MTEvent*> myqueue;
    QUICScheduler();
    MTEvent* GetNextEvent();
    void AddEvent(MTEvent*);
    MTEvent* CreateSendEvent(int, long);
    MTEvent* CreateReceiveEvent(int, long, Packet* pkg);
    bool isEmpty();
    };
}

#endif