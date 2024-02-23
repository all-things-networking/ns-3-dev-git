#include "ns3/mt-event.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
namespace ns3{

//////////////////////////// Sender ////////////////////////////

class MTHeader;
class MTEvent;
class SendEvent: public MTEvent {
    public:
    int flow_id;
    SendEvent();
    SendEvent(int flow_id);
};

class AckEvent : public MTEvent {
  public:
    int flow_id;
    std::vector<int> pkt_nums; //list<int> pkt_nums;
    int largest;
    int ECT0_Count;
    int ECT1_Count;
    int ECN_CE_Count;
    AckEvent();
    AckEvent(int flow_id);
};

class AddDataEvent : public MTEvent {
    public:
    bool server_side;
    bool direction;
    std::string data;//stream data;
    AddDataEvent();
    AddDataEvent(bool server_side, bool direction, std::string data);
};

// This one would be modified after finishing all the implementation and before testing
class SenderEventCreator
{
    public:
    // These events are used mainly for testing right now - it creates the specific event based
    // on input
    MTEvent* CreateAddDataEvent(bool server_side, bool direction, std::string data);
    MTEvent* CreateSendPacketEvent(int flow_id);
    MTEvent* CreateACKPacketEvent(int flow_id);
};
////////////////////////////////////////////////////////////////

//////////////////////////// Receiver //////////////////////////
/**
 * NOT SURE IF IT is useful for our translation: Event for application layer updates
 */
class AppUpdateEvent : public MTEvent
{
    public:
    AppUpdateEvent();
    AppUpdateEvent(long time, int flow_id);
};

/**
 * TIMER in TEL DSL is currently empty
 */
class TimerEvent : public MTEvent
{
    public:
    TimerEvent();
    //TimerEvent(long time, int flow_id);
};

/**
 * Congestion in TEL DSL
 */
class CongestionEvent : public MTEvent
{
    public:
    int time_sent;
    CongestionEvent();
    CongestionEvent(int time_sent);
};

// This one would be modified after finishing all the implementation and before testing
class ReceiverEventCreator
{
    public:
    // These events are used mainly for testing right now - it creates the specific event based
    // on input
    MTEvent* CreateReceiveEvent(int, long, Packet* packet);
    Packet* CreateFakePacket(std::vector<std::string>& data,
                                int packetNumber,
                                bool setFinBit,
                                int streamOffset,
                                int streamContentOffset = 0);
};

/////////////////////////////////////////////////////////////
}