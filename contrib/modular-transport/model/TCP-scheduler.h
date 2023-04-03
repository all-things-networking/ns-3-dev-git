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
    //TODO: Need pointer to MT
    TCPScheduler();
    //void OpsBeforeSend(); // Compute Hash
    //void OpsAfterRecieved(); //Verify Hash, then return a event
    //MTEvent GenerateEventOnRecive(MTHeader); //Produce event to be added to EventQueue
    //MTEvent GenerateEventOnInitate(MTHeader);
    MTEvent* GetNextEvent();
    void AddEvent(MTEvent*, ModularTransport* mt);
    void enqueue(MTEvent*);
    MTEvent* CreateSendEvent(int, long);
    MTEvent* CreateAckEvent(int, int);
    bool isEmpty();
    };
}

#endif
