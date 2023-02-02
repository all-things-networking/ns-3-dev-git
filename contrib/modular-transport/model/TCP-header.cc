#include "TCP-header.h"
#include "mt-header.h"
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
