#include "ns3/mt-state.h"
#include "ns3/mt-context.h"
#include "QUICSender-Context.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICContext::QUICContext(int id)//, uint32_t timer_duration) {
{
    this->data = NULL;
    this->PacketBuffer = new QUICPacketBuffer;
}
QUICContext::~QUICContext(){}
} // namespace ns3
