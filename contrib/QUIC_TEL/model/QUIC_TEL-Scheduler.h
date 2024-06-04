#ifndef QUIC_SCHEDULER_H
#define QUIC_SCHEDULER_H
#include "QUIC_TEL-Event.h"

#include "ns3/mt-scheduler.h"

#include <queue>
#include <vector>

namespace ns3
{
class MTScheduler;
class MTEvent;
class MTHeader;
class QUICPacketBuffer;

class QUICEvent;
class SendEvent;
class AddDataEvent;
class AckEvent;

// This scheduler doesn't correspond to latest TEL specification of QUIC
class QUICScheduler : public MTScheduler
{
  public:
    std::queue<SendEvent*> send_queue;
    std::queue<AddDataEvent*> add_queue;
    std::queue<AckEvent*> ack_queue;

    void AddEvent(MTEvent*);
    bool isEmpty();

    QUICScheduler();
    // keep this as MTEvent for now to keep the consistency with dispatcher
    MTEvent* GetNextEvent(); // event_t next_event(queue_t send_queue, queue_t ack_queue, queue_t
                             // add_queue)
    bool enqueue(QUICEvent* new_event); //  bool enqueue(queue_t ack_queue, event_t new_event)
    int add_drop();             // int add_drop(queue_t add_queue, event_t new_event)
    int send_drop();
    int ack_drop();
    bool isSendEmpty();
};
} // namespace ns3

#endif