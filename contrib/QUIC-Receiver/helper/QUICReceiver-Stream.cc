#include "QUICReceiver-Stream.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICStream::QUICStream(int id)//, uint32_t timer_duration)
    : id{id}    
{
}
QUICStream::~QUICStream(){}
} // namespace ns3