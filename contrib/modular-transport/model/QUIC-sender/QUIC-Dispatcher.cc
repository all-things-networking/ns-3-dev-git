#include "mt-dispatcher.h"
#include "QUIC-Dispatcher.h"
#include "mt-event.h"
#include "mt-eventprocessor.h"
namespace ns3{

// Constructor
// TODO: 
QUICDispatcher::QUICDispatcher(){}

MTEventProcessor* QUICDispatcher::dispatch(MTEvent anything){

    // TODO: Right now just picking a random processor
    // need to update to choose the correct processor based on the event
    MTEventProcessor* ChosenProcessor = new SendIfPossible();
    return ChosenProcessor;
}

void QUICDispatcher::AddProcessor(MTEventProcessor* processor)
{
  m_processors.push_back(processor);
}

}
