class MTScheduler;
class MTEvent;
class TCPscheduler: public MTScheduler{
    void OpsBeforeSend(); // Compute Hash
    void OpsAfterRecieved(); //Verify Hash, then return a event
    MTEvent GenerateEventOnRecive(MTHeader); //Produce event to be added to EventQueue
    MTEvent GenerateEventOnInitate(MTHeader);
    MTEvent GetNextEvent();
    }
}
