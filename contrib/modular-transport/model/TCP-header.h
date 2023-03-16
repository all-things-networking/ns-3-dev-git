#ifndef MT_TCP_HEADER_H
#define MT_TCP_HEADER_H
namespace ns3{
class MTHeader;
//#Modular transport TCP header
class MTTCPHeader : public MTHeader {
    public:
        uint32_t seqnum; //32 bits
        uint32_t acknum; // 32 bits
        int DOffset; //4 bits
        int Reserved; //4 bits
        uint32_t ControlBits; //1 bit each
        uint32_t urgentbit = 1;
        uint32_t ackbit = 2;
        uint32_t PushFlag = 4;
        int Window; //16 bits
        int CheckSum; //16 bits
        int UrgentPointer; // 16 bits
        int* Options; // (DOFFset - 5 ) *32
        char* data;
        MTTCPHeader();
        ~MTTCPHeader();
        TypeId GetInstanceTypeId() const override;
        void Print(std::ostream& os) const override;
        uint32_t GetSerializedSize() const override;
        void Serialize(Buffer::Iterator start) const override;
        uint32_t Deserialize(Buffer::Iterator start) override;
        int GenPseudoHeader();
        int ComputeCheckSum();
        void OpsBeforeSend(); // Compute Hash, call by processor before send?
        void OpsAfterRecieved(); //Just produce event, leave verify hash to processor?
    private:
          uint32_t m_f1;

};
}
#endif
