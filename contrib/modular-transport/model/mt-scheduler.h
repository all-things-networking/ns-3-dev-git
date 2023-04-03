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
    std::queue<MTEvent*> myqueue;
    MTScheduler(){}
    // virtual void OpsBeforeSend(); // Compute Hash
   // virtual void OpsAfterRecieved(); //Verify Hash, then return a event
    //virtual MTEvent GenerateEventOnRecive(MTHeader); //Produce event to be added to EventQueue
    //virtual MTEvent GenerateEventOnInitate(MTHeader);
    virtual MTEvent* GetNextEvent()=0;
    virtual void AddEvent(MTEvent* newEvent, ModularTransport* mt){
        if(this->myqueue.empty()){
            this->enqueue(newEvent);
            mt->Mainloop();
        }else{
            this->enqueue(newEvent);
        }
    } // //TODO: call enqueu, Restart the loop if stopped.

    virtual void enqueue(MTEvent*) =0;

    virtual MTEvent* CreateSendEvent(int, long)=0;
    virtual MTEvent* CreateAckEvent(int, int)=0;
    virtual bool isEmpty()=0;
};
}
#endif
