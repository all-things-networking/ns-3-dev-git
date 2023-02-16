#ifndef MT_TCP_SCHEDULER_H
#define MT_TCP_SCHEDULER_H
#include "mt-scheduler.h"
namespace ns3{
class MTScheduler;
class MTEvent;
class MTHeader;
class TCPscheduler: public MTScheduler{
    public:
    std::queue<MTEvent> myqueue;
    TCPscheduler();
    //void OpsBeforeSend(); // Compute Hash
    //void OpsAfterRecieved(); //Verify Hash, then return a event
    //MTEvent GenerateEventOnRecive(MTHeader); //Produce event to be added to EventQueue
    //MTEvent GenerateEventOnInitate(MTHeader);
    MTEvent GetNextEvent();
    void AddEvent(MTEvent);
    MTEvent CreateSendEvent(int, long);
    bool isEmpty();
    };
}

#endif
