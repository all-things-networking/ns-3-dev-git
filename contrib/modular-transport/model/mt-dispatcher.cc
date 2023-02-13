#include "mt-dispatcher.h"
#include "mt-event.h"
namespace ns3{
MtEventProcessor TCPDispatcher::dispatch(MTevent){
    MtEventProcessor ChosenProcessor = SendIfPossible();
    return ChosenProcessor;
}
}
