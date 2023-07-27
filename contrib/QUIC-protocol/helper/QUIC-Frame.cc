#include "QUIC-Frame.h"

#include <ctime>
#include <map>
#include <sstream>
#include <iostream>
#include <utility>   // std::pair

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

// Constructors, deprecating soon
QUICFrameHeader::QUICFrameHeader(
    uint32_t streamID, uint32_t offset, uint32_t length, FrameType frameType, bool finBit,
    uint32_t largestACKed, uint32_t ackRangeCount, uint32_t firstACKRange
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

void QUICFrameHeader::SerializeStreamFrame(Buffer::Iterator i) const {
    i.WriteHtonU32(streamID);
    i.WriteHtonU32(offset);
    i.WriteHtonU32(length);
    i.WriteHtonU32(fin);
}

void QUICFrameHeader::SerializeACKFrame(Buffer::Iterator i) const {
    i.WriteHtonU32(largestACKed);
    i.WriteHtonU32(ackRangeCount);
    i.WriteHtonU32(firstACKRange);
    for(int j = 0; j < ackRangeCount; j++ ){
        uint32_t gap = ACKRanges[ j ].first;
        uint32_t ackRange = ACKRanges[ j ].second;
        i.WriteHtonU32( gap );
        i.WriteHtonU32( ackRange );
    }
}

void QUICFrameHeader::Serialize(Buffer::Iterator start) const {
    // std::cout << "Serialize CALLED::::: " << streamID << " " << offset << std::endl;
    Buffer::Iterator i = start;
    i.WriteHtonU32(frameType);
    if ( frameType == FrameType::STREAM ){
        SerializeStreamFrame(i);
    } else if ( frameType == FrameType::ACK ){
        SerializeACKFrame(i);
    }
}

void QUICFrameHeader::DeserializeStreamFrame(Buffer::Iterator i){
    streamID = i.ReadNtohU32();
    offset = i.ReadNtohU32();
    length = i.ReadNtohU32();
    fin = i.ReadNtohU32();
    currentFrameSize = 20;
}

void QUICFrameHeader::DeserializeACKFrame(Buffer::Iterator i){
    largestACKed = i.ReadNtohU32();
    ackRangeCount = i.ReadNtohU32();
    firstACKRange = i.ReadNtohU32();
    currentFrameSize = 16;
    // use AckRangeCount to read out ackranges
    for(int j = 0; j < ackRangeCount; j++ ){
        currentFrameSize += 8;
        uint32_t gap = i.ReadNtohU32();
        uint32_t ackRange = i.ReadNtohU32();
        ACKRanges.push_back( std::make_pair(gap, ackRange) );
    }
}

uint32_t QUICFrameHeader::Deserialize(Buffer::Iterator start){
    Buffer::Iterator i = start;
    frameType = i.ReadNtohU32();
    if ( frameType == FrameType::STREAM ){
        DeserializeStreamFrame(i);
    } else if ( frameType == FrameType::ACK ){
        DeserializeACKFrame(i);
    }
    return GetSerializedSize();
}

void QUICFrameHeader::setACKFrameHeader(uint32_t largestACKed, uint32_t ackRangeCount, uint32_t firstACKRange){
    this->frameType = FrameType::ACK;
    this->largestACKed = largestACKed;
    this->ackRangeCount = ackRangeCount;
    this->firstACKRange = firstACKRange;

    this->currentFrameSize = 16;
}

void QUICFrameHeader::setMaxStreamFrameHeader(){
    // stream ID
    // Max absolute byte offset of a stream

    // A receiver could determine the flow control offset to be advertised 
    // based on the current offset of data consumed on that stream.   
}

void QUICFrameHeader::setMaxDataFrameHeader(){
    // for the whole connection
    // the maximum data can be send
}

void QUICFrameHeader::ACKRangesPush(uint32_t gap, uint32_t length){
    ACKRanges.push_back( std::make_pair(gap, length) );
    this->currentFrameSize += 8;
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
