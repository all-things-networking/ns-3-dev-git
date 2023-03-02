#include "mt-dispatcher.h"
#include "QUIC-Dispatcher.h"
#include "mt-event.h"
#include "QUIC-Event.h"
#include "mt-eventprocessor.h"
#include "./QUIC-sender/QUIC-SendIfPossible.h"
namespace ns3{
QUICDispatcher::QUICDispatcher(){}

MTEventProcessor* QUICDispatcher::dispatch(MTEvent anything){
    MTEventProcessor* ChosenProcessor = new QUICSendIfPossible();
    return ChosenProcessor;
}
}
