#include "QUIC-StreamContext.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICStreamContext::QUICStreamContext(int id)//, uint32_t timer_duration)
    : id{id}    
{
}
QUICStreamContext::~QUICStreamContext(){}
} // namespace ns3
