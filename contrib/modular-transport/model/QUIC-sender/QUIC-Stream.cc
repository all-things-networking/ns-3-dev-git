#include "QUIC-Stream.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICStream::QUICStream(int id)//, uint32_t timer_duration)
    : id{id}    
{
}
QUICStream::~QUICStream(){}

void QUICStream::AddFrame(QUICFrame* frame)
{
    frames.emplace_back(frame);
}

QUICFrame* QUICStream::RemoveFrame()
{
    QUICFrame* currFrame = frames.front();
    frames.pop_front();
    return currFrame;
}

} // namespace ns3
