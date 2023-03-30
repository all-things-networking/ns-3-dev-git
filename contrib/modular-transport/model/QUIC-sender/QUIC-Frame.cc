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

QUICFrame::QUICFrame()
{
}

QUICFrame::~QUICFrame()
{
}

int QUICFrame::GetSize()
{
    return this->data->GetSize();
}




} // namespace ns3
