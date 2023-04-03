//Schedular add generate events, and add them to queue?
#ifndef MT_SCHEDULER_H
#define MT_SCHEDULER_H
#include <queue>
namespace ns3{
class MTEvent;
class MTEventQueue;
class MTHeader;
class ModularTransport;
class MTScheduler{
    public:
    MTScheduler(){}
    void SetModularTransport(ModularTransport* mt);
    virtual MTEvent* GetNextEvent()=0;
    void AddEvent(MTEvent* newEvent);
    virtual void enqueue(MTEvent*) =0;
    virtual MTEvent* CreateSendEvent(int, long)=0;
    virtual MTEvent* CreateAckEvent(int, int)=0;
    virtual bool isEmpty()=0;
    protected:
        ModularTransport * mt = NULL;

};
}
#endif
