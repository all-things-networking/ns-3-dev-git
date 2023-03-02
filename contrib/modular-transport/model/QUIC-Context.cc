#include "mt-state.h"
#include "mt-context.h"
#include "QUIC-Context.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICContext::QUICContext(int id)//, uint32_t timer_duration)
    : data{NULL}    
{
}
QUICContext::~QUICContext(){}
} // namespace ns3
