#ifndef QUIC_SCHEDULER_H
#define QUIC_SCHEDULER_H
#include "ns3/mt-scheduler.h"
#include "QUIC-Event.h"
#include "../helper/QUIC-PacketBuffer.h"
#include <queue>
#include <vector>
namespace ns3{
class MTScheduler;
class MTEvent;
class MTHeader;
class QUICPacketBuffer;
class QUICScheduler: public MTScheduler{
    public:
    std::queue<MTEvent*> myqueue;
    QUICScheduler();
    MTEvent* GetNextEvent();
    void AddEvent(MTEvent*);

    //////////////////////////// Sender //////////////////////////
    MTEvent* CreateSendEvent(int, long);
    MTEvent* SendPacket(int flow_id, long time);
    MTEvent* AddData(int flow_id, long time, std::string text, int stream);
    MTEvent* ACKPacket(int flow_id, long time, int packetNum);
    //////////////////////////////////////////////////////////////

    //////////////////////////// Receiver ////////////////////////
    Packet* CreateFakePacket();
    MTEvent* CreateReceiveEvent(int, long, Packet* pkt);
    //////////////////////////////////////////////////////////////

    bool isEmpty();
    };
}

#endif