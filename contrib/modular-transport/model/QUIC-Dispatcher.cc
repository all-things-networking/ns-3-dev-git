#include "mt-dispatcher.h"
#include "QUIC-Dispatcher.h"
#include "mt-event.h"
#include "QUIC-Event.h"
#include "mt-eventprocessor.h"
#include "./QUIC-sender/QUIC-SendIfPossible.h"
#include "./QUIC-sender/QUIC-StreamHandler.h"
#include "./QUIC-sender/QUIC-LossDetection.h"
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
