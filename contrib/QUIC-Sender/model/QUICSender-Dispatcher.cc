#include "ns3/mt-dispatcher.h"
#include "ns3/mt-eventprocessor.h"
#include "ns3/mt-event.h"

#include "QUICSender-Dispatcher.h"
#include "QUICSender-Event.h"

#include "../helper/QUICSender-SendIfPossible.h"
#include "../helper/QUICSender-StreamHandler.h"
#include "../helper/QUICSender-LossDetection.h"
#include <iostream>
namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent* event){
    std::vector<MTEventProcessor*> ChosenProcessors;
    if (event->type == EventType::STREAM_EVENT) {
        MTEventProcessor* ChosenProcessor = new QUICStreamHandler();
        ChosenProcessors.push_back(ChosenProcessor);
    }

    if (event->type == EventType::RESPONSE_EVENT) {
        MTEventProcessor* ChosenProcessor = new QUICLossDetection();
        ChosenProcessors.push_back(ChosenProcessor);
    }

    // MTEventProcessor* ChosenProcessor = new QUICSendIfPossible();
    // return ChosenProcessor;
    return ChosenProcessors;
}
}
