#include "QUIC-Frame.h"

#include <ctime>
#include <map>

namespace ns3
{

QUICFrame::QUICFrame(FrameType type, FrameFields fields)
{
    this->type = type;
    this->fields = fields;
}

QUICFrame::QUICFrame(int size)
{
    this->size = size;
}

QUICFrame::QUICFrame()
{
}

QUICFrame::~QUICFrame()
{
}

int QUICFrame::GetSize()
{
    return this->size;
}




} // namespace ns3
