#include "QUICSender-Stream.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICStream::QUICStream(int id)//, uint32_t timer_duration)
    : id{id}    
{
}
QUICStream::~QUICStream(){}

void QUICStream::AddToDataBuffer(std::string data)
{
    databuffer = databuffer + data;
}

} // namespace ns3
