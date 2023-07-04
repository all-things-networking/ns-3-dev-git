#include "QUIC-Frame.h"

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

uint32_t QUICFrame::GetSize()
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

// looks like the Serialize is used in Sender
void QUICFrame::Serialize(uint8_t* buffer){


    // currently only for ACK packets
    if (type == ACK){


        ACKFrameFields* f = static_cast<ACKFrameFields*>(fields);
        uint32_t i = f->LargestACKed;
        // probably needs a better way to convert longer integers to bytes
        buffer[0] = i >> 24;
        buffer[1] = i >> 16;
        buffer[2] = i >>  8;
        buffer[3] = i;
    }
}




} // namespace ns3
