#include "QUICSender-Frame.h"

#include <ctime>
#include <map>
#include <sstream>

namespace ns3
{

QUICFrame::QUICFrame(FrameType type, FrameFields * fields)
{
    this->type = type;
    this->fields = fields;
}

QUICFrame::QUICFrame(std::string frame)
{
    std::stringstream ssFrame(frame);
    std::string header;
    std::string data;

    std::getline(ssFrame, header, '_');
    std::getline(ssFrame, data, '_');

    // TODO: need to parse the header and data

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

std::string QUICFrame::GetData()
{
    return "";
}

// TODO: super simplified right now, will need to modify later
std::string QUICFrame::generateHeader()
{
    if (this->type == FrameType::STREAM) {
        StreamFrameFields streamFields = static_cast<StreamFrameFields&>(*this->fields);
        return "HEADER: " + std::to_string(streamFields.StreamID);
    }
    
    return "";
}




} // namespace ns3
