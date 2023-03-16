#include "QUIC-dispatcher.h"
#include "mt-event.h"
#include "mt-eventprocessor.h"
#include "QUIC-eventprocessor.h"

namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent* anything){
    std::vector<MTEventProcessor*> ChosenProcessors;
    ChosenProcessors.push_back(new QUICPacketDemultiplexer());
    ChosenProcessors.push_back(new QUICBufferManagement());
    ChosenProcessors.push_back(new QUICAckHandler());
    return ChosenProcessors;
}
}