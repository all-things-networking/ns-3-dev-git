#ifndef MT_QUIC_SCHEDULER_H
#define MT_QUIC_SCHEDULER_H
#include "mt-scheduler.h"
#include "QUIC-event.h"
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