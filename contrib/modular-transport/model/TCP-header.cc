#include "mt-header.h"
#include "TCP-header.h"
namespace ns3{
int MTTCPHeader::GenPseudoHeader() {
    return 0;
}
int MTTCPHeader::ComputeCheckSum() {
    //To be implemented
    return 0;
}
void MTTCPHeader::OpsBeforeSend() {
    this->CheckSum = this->ComputeCheckSum();
    //Acknum should be set already, it's prcoesser's job


}
void MTTCPHeader::OpsAfterRecieved() { //Return A event to be added to Queue?
    int verifyChecksum = this->ComputeCheckSum();
    if (verifyChecksum != this->CheckSum) {
        //Check sum failed
    }//else continue
    //return a event
}
MTTCPHeader::MTTCPHeader(){
    seqnum = 0;
    acknum = 0;
    ControlBits = 0;
}
MTTCPHeader::~MTTCPHeader(){

}
TypeId
MTTCPHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}
void
MTTCPHeader::Print(std::ostream& os) const
{
    os << "(" << seqnum << ")";
}

uint32_t
MTTCPHeader::GetSerializedSize() const
{
    return 12;
}
void
MTTCPHeader::Serialize(Buffer::Iterator start) const
{
    Buffer::Iterator i = start;
    i.WriteHtonU32(seqnum);
    i.WriteHtonU32(acknum);
    i.WriteHtonU32(ControlBits);
}
uint32_t
MTTCPHeader::Deserialize(Buffer::Iterator start)
{
    Buffer::Iterator i = start;
    seqnum = i.ReadNtohU32();
    acknum = i.ReadNtohU32();
    ControlBits = i.ReadNtohU32();
    return GetSerializedSize();
}

}
