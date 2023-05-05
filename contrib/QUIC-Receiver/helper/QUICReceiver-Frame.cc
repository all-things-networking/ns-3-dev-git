#include "QUICReceiver-Frame.h"


#include <ctime>
#include <map>


namespace ns3
{


QUICFrame::QUICFrame(FrameType type, FrameFields* fields)
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


uint32_t QUICFrame::GetSize(){
    // TODO: implement this function
    return 4;
}


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