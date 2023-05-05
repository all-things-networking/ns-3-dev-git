#include "QUICReceiver-Dispatcher.h"
#include "ns3/mt-event.h"
#include "ns3/mt-eventprocessor.h"
#include "../helper/QUICReceiver-EventProcessor.h"

namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent* anything){

    // for now, just returning the 3 processors to process a receivepkg event
    std::vector<MTEventProcessor*> ChosenProcessors;
    ChosenProcessors.push_back(new QUICPacketDemultiplexer());
    ChosenProcessors.push_back(new QUICBufferManagement());
    ChosenProcessors.push_back(new QUICAckHandler());
    return ChosenProcessors;
}
}