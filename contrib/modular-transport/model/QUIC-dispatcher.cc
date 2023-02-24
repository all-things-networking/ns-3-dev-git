#include "QUIC-dispatcher.h"
#include "mt-event.h"
#include "mt-eventprocessor.h"

namespace ns3{
QUICDispatcher::QUICDispatcher(){}

std::vector<MTEventProcessor*> QUICDispatcher::dispatch(MTEvent anything){
    std::vector<MTEventProcessor*> ChosenProcessors;
    return ChosenProcessors;
}
}