#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/mt-event.h"

#include "QUIC_TEL-Scheduler.h"
#include "QUIC_TEL-Event.h"

#include <string>

namespace ns3{
QUICScheduler::QUICScheduler(){
}

bool QUICScheduler::Enqueue(MTEvent* event){
    QUICEvent* new_event = dynamic_cast<QUICEvent*>(event);
    if (new_event->type == SEND) {
        send_queue.push(dynamic_cast<SendEvent*>(new_event));
        return 1;
    }
    if (new_event->type == ADD_DATA) {
        add_queue.push(dynamic_cast<AddDataEvent*>(new_event));
        return 1;
    }
    if (new_event->type == ACK) {
        ack_queue.push(dynamic_cast<AckEvent*>(new_event));
        return 1;
    }
    return 0;
}

bool
QUICScheduler::isSendEmpty() {
    return this->send_queue.empty();
}

MTEvent* QUICScheduler::NextEvent() {
    if (add_queue.size() > ack_queue.size() / 2) {
        QUICEvent* top = send_queue.front();
        ack_queue.pop();
        return (top);
    }
    if (send_queue.size() < ack_queue.size()) {
        QUICEvent* top = ack_queue.front();
        ack_queue.pop();
        return (top);
    }
    QUICEvent* top = add_queue.front();
    add_queue.pop();
    return (top);
}

int QUICScheduler::add_drop() {
    int index = add_queue.size() - 1;
    return index;
}

int QUICScheduler::ack_drop() {
    int index = ack_queue.size() - 1;
    return index;
}

int
QUICScheduler::send_drop() {
    int index = send_queue.size() - 1;
    return index;
}

}