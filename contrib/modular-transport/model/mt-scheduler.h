//Schedular add generate events, and add them to queue?
class MTEvent;
class MTEventQueue;
class MTHeader
class MTSchedular{
    struct MTEventQueue;
    MTSchedular();
    virtual void OpsBeforeSend(); // Compute Hash
    virtual void OpsAfterRecieved(); //Verify Hash, then return a event
    virtual MTEvent GenerateEventOnRecive(MTHeader); //Produce event to be added to EventQueue
    virtual MTEvent GenerateEventOnInitate(MTHeader);
    virtual MTEvent GetNextEvent();
}
