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

// TODO: super simplified right now, will need to modify later
std::string QUICFrame::generateHeader()
{
    if (this->type == FrameType::STREAM) {
        StreamFrameFields streamFields = static_cast<StreamFrameFields&>(this->fields);
        return "HEADER: " + std::to_string(streamFields.StreamID) + "-" + std::to_string(streamFields.BufferIndex);
    }
    
    return "";
}




} // namespace ns3
