#include "QUIC_TEL-Dispatcher.h"
#include "QUIC_TEL-Event.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"


#include "../helper/QUIC_TEL-sendProcessor.h"
#include "../helper/QUIC_TEL-selectDataProcessor.h"


#include <iostream>

namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent* event){

    //////////////////////////// Sender ////////////////////////////
    std::vector<MTEventProcessor*> ChosenProcessors;
    QUICEvent* quicEvent = dynamic_cast<QUICEvent*>(event);
    MTEventProcessor* ChosenProcessor;


    // If we have a SEND_PACKET event we will send the packet
    if (quicEvent->type == EventType::SEND)
    {
        ChosenProcessor = new QUICSendProcessor();
    }

    // If we have a ADD_DATA event, create a dataFrame and send it
    if (quicEvent->type == EventType::ADD_DATA)
    {
        ChosenProcessor = new selectDataProcessor();
    }

    // If we have an ACK event, create ?
    if (quicEvent->type == EventType::ACK) {
        //ChosenProcessor = new QUICAck();
    }

    // If we have a TIMER event, create ?
    if (quicEvent->type == EventType::TIMER) {
        //ChosenProcessor = new QUICtimer();
    }

    // If we have a CONGESTION event, create ?
    if (quicEvent->type == EventType::CONGESTION) {
        //ChosenProcessor = new QUICcongestion();
    }

    ChosenProcessors.push_back(ChosenProcessor);
    return ChosenProcessors;
}
}