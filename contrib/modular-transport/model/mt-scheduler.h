//Schedular add generate events, and add them to queue?
#ifndef MT_SCHEDULER_H
#define MT_SCHEDULER_H
namespace ns3{
class MTEvent;
class MTEventQueue;
class MTHeader;
class MTScheduler{
    public:
    MTScheduler();
    // virtual void OpsBeforeSend(); // Compute Hash
   // virtual void OpsAfterRecieved(); //Verify Hash, then return a event
    virtual MTEvent GenerateEventOnRecive(MTHeader); //Produce event to be added to EventQueue
    virtual MTEvent GenerateEventOnInitate(MTHeader);
    virtual MTEvent GetNextEvent();
    virtual MTEvent AddEvent(MTEvent);
    virtual MTEvent CreateSendEvent(int, long);
    virtual bool isEmpty();
};
}
#endif
