#include "mt-dispatcher.h"
#include "mt-event.h"
#include "mt-eventprocessor.h"
namespace ns3{
TCPDispatcher::TCPDispatcher(){}

MTEventProcessor TCPDispatcher::dispatch(MTEvent anything){
    MtEventProcessor ChosenProcessor = SendIfPossible();
    return ChosenProcessor;
}
}
