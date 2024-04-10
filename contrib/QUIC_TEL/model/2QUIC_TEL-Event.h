#ifndef QUIC_EVENT_H
#define QUIC_EVENT_H
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/mt-event.h"
#include "ns3/node.h"
using namespace std;
using namespace ns3;

enum EventType
{
    SEND,
    ACK,
    TIMER,
    ADD_DATA,
    CONGESTION,
    EMPTY // THERE is no EMPTY in TEL, add this here temporarily
};

// This is the base event for QUIC. It uses an EventType for further filtering in the dispatcher
class QUICEvent : public MTEvent
{
  public:
    QUICEvent();
    QUICEvent(long time, int flow_id, EventType type = EventType::EMPTY);
    EventType type = EventType::EMPTY;
};

class SendEvent : public QUICEvent
{
    int flow_id;
};

class AckEvent : public QUICEvent
{
    int flow_id;
    vector<int> pkt_nums;
    int largest;
    int ECT0_Count;
    int ECT1_Count;
    int ECN_CE_Count;
};

class TimerEvent : public QUICEvent
{
};

class AddDataEvent : public QUICEvent
{
    bool server_side;
    bool direction;
    vector<char> data;
};

class CongestionEvent : public QUICEvent
{
    int time_sent;
};
#endif