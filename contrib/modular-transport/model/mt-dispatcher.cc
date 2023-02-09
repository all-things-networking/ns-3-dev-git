#include "mt-dispatcher"
#include "mt-event"
namespace ns3{
MtEventProcessor TCPDispatcher::dispatch(MTevent){
    MtEventProcessor ChosenProcessor = SendIfPossible();
    return ChosenProcessor;
}
}
