#include "QUIC-Frame.h"

#include <ctime>
#include <map>
#include <sstream>
#include <iostream>

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
    std::cout << "QUICFRAME SERIALIZE CALLED" << std::endl;


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

// Constructors
QUICFrameHeader::QUICFrameHeader(
    uint32_t streamID, uint32_t offset, uint32_t length, FrameType frameType, bool finBit,
    uint32_t largestAcked, uint32_t ackRangeCount, uint32_t firstACKRange
    ){
    this->offset = offset;
    this->streamID = streamID;
    this->length = length;
    this->frameType = frameType;
    this->fin = finBit;

    this->largestACKed = largestACKed;
    this->ackRangeCount = ackRangeCount;
    this->firstACKRange = firstACKRange;


    if ( frameType == FrameType::STREAM ) {
        this->currentFrameSize = 20;
    } else if ( frameType == FrameType::ACK ) {
        this->currentFrameSize = 16;
    } 
    // std::cout << " QUICFrameHeader Constructor ";
    // std::cout << offset << " " << streamID << std::endl;
}

QUICFrameHeader::QUICFrameHeader(){
}

TypeId QUICFrameHeader::GetInstanceTypeId() const {
    return GetTypeId();
}

void QUICFrameHeader::Print(std::ostream& os) const {
    os << "Hello World from Print" << std::endl;
}

uint32_t QUICFrameHeader::GetSerializedSize() const {
    // return 20; // TODO: Need to Change
    return this->currentFrameSize;
}

void QUICFrameHeader::Serialize(Buffer::Iterator start) const {
    // std::cout << "Serialize CALLED::::: " << streamID << " " << offset << std::endl;
    Buffer::Iterator i = start;
    i.WriteHtonU32(frameType);
    if ( frameType == FrameType::STREAM ){
        i.WriteHtonU32(streamID);
        i.WriteHtonU32(offset);
        i.WriteHtonU32(length);
        i.WriteHtonU32(fin);
    } else if ( frameType == FrameType::ACK ){
        i.WriteHtonU32(largestACKed);
        i.WriteHtonU32(ackRangeCount);
        i.WriteHtonU32(firstACKRange);
    }
}

uint32_t QUICFrameHeader::Deserialize(Buffer::Iterator start){
    Buffer::Iterator i = start;
    frameType = i.ReadNtohU32();
    if ( frameType == FrameType::STREAM ){
        streamID = i.ReadNtohU32();
        offset = i.ReadNtohU32();
        length = i.ReadNtohU32();
        fin = i.ReadNtohU32();
        currentFrameSize = 20;
    } else if ( frameType == FrameType::ACK ){
        largestACKed = i.ReadNtohU32();
        ackRangeCount = i.ReadNtohU32();
        firstACKRange = i.ReadNtohU32();
        currentFrameSize = 16;
    }
    return GetSerializedSize();
}

///////////////// try out new way /////////////////////
// QUICStreamFrameHeader::QUICStreamFrameHeader( uint32_t streamID, uint32_t offset, uint32_t length, FrameType frameType, bool finBit ){
//     this->offset = offset;
//     this->streamID = streamID;
//     this->length = length;
//     this->frameType = frameType;
//     this->fin = finBit;

//     // std::cout << " QUICFrameHeader Constructor ";
//     // std::cout << offset << " " << streamID << std::endl;
// }

// QUICStreamFrameHeader::QUICStreamFrameHeader(){
// }

// TypeId QUICStreamFrameHeader::GetInstanceTypeId() const {
//     return GetTypeId();
// }

// void QUICStreamFrameHeader::Print(std::ostream& os) const {
//     os << "Hello World from Print" << std::endl;
// }

// uint32_t QUICStreamFrameHeader::GetSerializedSize() const {
//     return this->currentFrameSize;
// }

// void QUICStreamFrameHeader::Serialize(Buffer::Iterator start) const {
//     // std::cout << "Serialize CALLED::::: " << streamID << " " << offset << std::endl;
//     Buffer::Iterator i = start;
//     i.WriteHtonU32(frameType);
//     i.WriteHtonU32(streamID);
//     i.WriteHtonU32(offset);
//     i.WriteHtonU32(length);
//     i.WriteHtonU32(fin);
// }

// uint32_t QUICStreamFrameHeader::Deserialize(Buffer::Iterator start){
//     Buffer::Iterator i = start;
//     frameType = i.ReadNtohU32();
//     streamID = i.ReadNtohU32();
//     offset = i.ReadNtohU32();
//     length = i.ReadNtohU32();
//     fin = i.ReadNtohU32();
//     currentFrameSize = 20;
//     return GetSerializedSize();
// }
///////////////// try out new way end /////////////////////


} // namespace ns3
