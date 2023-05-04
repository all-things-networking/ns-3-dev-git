#include "ns3/mt-dispatcher.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-event.h"

#include "QUICSender-Dispatcher.h"
#include "QUICSender-Event.h"

#include "../helper/QUIC-SendIfPossible.h"
#include "../helper/QUIC-StreamHandler.h"
#include "../helper/QUIC-LossDetection.h"
#include <iostream>
namespace ns3{
QUICDispatcher::QUICDispatcher(){}

MTEventProcessor* QUICDispatcher::dispatch(MTEvent* event){
    if (event->type == EventType::STREAM_EVENT) {
        MTEventProcessor* ChosenProcessor = new QUICStreamHandler();
        return ChosenProcessor;
    }

    if (event->type == EventType::RESPONSE_EVENT) {
        MTEventProcessor* ChosenProcessor = new QUICLossDetection();
        return ChosenProcessor;
    }

    // MTEventProcessor* ChosenProcessor = new QUICSendIfPossible();
    // return ChosenProcessor;
    return nullptr;
}
}
