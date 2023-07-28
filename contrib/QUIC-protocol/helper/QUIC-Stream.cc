#include "QUIC-Stream.h"
#include <ctime>
#include <map>

namespace ns3
{
QUICStream::QUICStream(int id)//, uint32_t timer_duration)
    : id{id}    
{
}
QUICStream::~QUICStream(){}

void QUICStream::AddToDataBuffer(std::string data)
{
    databuffer = databuffer + data;
    offset = databuffer.size();
}

void QUICStream::AddToDataBuffer(std::string data, unsigned long start){
    if ( start >= databuffer.size() ){
        // we miss some contents in between
        while (is_received.size() < start ) { 
            is_received.push_back( false );
            databuffer += "_"; // placeholder
        }

        databuffer += data;
        for(unsigned long i = 0; i < data.size(); i++ ) is_received.push_back( true );
    } else { // some missing contents is re-transmitted
        int missedBytes = 0;
        for(unsigned long i = 0; i < data.size(); i++ ) missedBytes += is_received[ start + i ];

        // if we actually missed the contents
        if ( missedBytes == 0 ){ 
            for( unsigned long i = 0; i < data.size(); i++ ){
                is_received[ start + i ] = true;
                databuffer[ start + i ] = data[ i ];
            }
        }
    }
    offset = databuffer.size();
}

void QUICStream::SetFin(bool finBit){
    fin = fin || finBit;
    size = (int)databuffer.size();
}

} // namespace ns3
