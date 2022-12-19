#include "mt-eventprocessor.h"
#include "mt-state.h"
#include "mt-event.h"

#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::min, std::max

namespace ns3
{
MtEventProcessor::MtEventProcessor()
{
}

TcpAckHandler::TcpAckHandler()
    : MtEventProcessor()
{
}

bool
TcpAckHandler::IsValidEvent(MtEvent e)
{
    MtEvent* ePtr = &e;
    return dynamic_cast<MtAckEvent*>(ePtr) != nullptr;
}

std::pair<std::vector<MtEvent>, MtContext>
TcpAckHandler::Process(MtEvent e, MtContext c)
{
    // Check if event is correct type before process
    if (!this->IsValidEvent(e)) {
        // return the input (do nothing)
        return std::make_pair(std::vector<MtEvent>{e}, c);
    }

    MtAckEvent* event = dynamic_cast<MtAckEvent*>(&e);
    uint32_t ackNum = event->m_ackNum;
    std::vector<MtEvent> result{};
    if (ackNum > c.m_Una && ackNum <= c.m_Nxt)
    {
        // acceptable ack
        c.m_dupAmount = 0;
        c.m_Una = ackNum;
    }
    else if (ackNum == c.m_Una)
    {
        // Cumulate duplicate ack count
        if (ackNum > c.m_dupSeq)
        {
            c.m_dupSeq = ackNum;
            c.m_dupAmount = 1;
        } 
        else if (ackNum == c.m_dupSeq)
        {
            c.m_dupAmount += 1;
        }
    }
    // Create event for checking if new data can be sent.
    result.push_back(MtSendEvent(e.m_id));
    // Create event for checking if congestion.
    result.push_back(MtCongNotifyEvent(e.m_id, ackNum));
    return std::make_pair(result, c);
}

TcpCongControl::TcpCongControl()
    : MtEventProcessor()
{
}

bool 
TcpCongControl::IsValidEvent(MtEvent e)
{
    MtEvent* ePtr = &e;
    return dynamic_cast<MtCongNotifyEvent*>(ePtr) != nullptr; 
}

std::pair<std::vector<MtEvent>, MtContext>
TcpCongControl::Process(MtEvent e, MtContext c)
{
    // Check if event is correct type before process
    if (!this->IsValidEvent(e)) {
        // return the input (do nothing)
        return std::make_pair(std::vector<MtEvent>{e}, c);
    }

    MtCongNotifyEvent* event = dynamic_cast<MtCongNotifyEvent*>(&e);
    std::vector<MtEvent> result{};
    uint32_t lastAckNum = event->m_lastAckNum;
    // Check if recovered successfully.
    if (lastAckNum > c.m_Una && lastAckNum <= c.m_Nxt && c.m_congPhase == TcpCongPhase::FAST_RECOVER)
    {
        // Just recover from packet lost, back to congestion avoidence
        c.m_dupAmount = 0;
        c.m_congPhase = MtCongPhase::CONG_AVOID;
        // RFC5681 Sec3.2.6
        c.m_cnwd = c.m_ssthresh;
    }
    // Detect & Trigger Loss
    else if (c.m_dupAmount == 3)
    {
        // Fast Retrainsmit
        c.m_congPhase = TcpCongPhase::FAST_RETRAINS;
        // RFC5681 Sec3.2 - 3 duplicate Acks triggers fast retrainsmit
        result.push_back(MtSendEvent(e.m_id)); // Send packet at c.m_Una again
        // RFC5681 Sec3.1 Equation (4)
        c.m_ssthresh = std::max((c.m_Nxt - c.m_Una) / 2, 2 * c.m_Smss);
        // RFC5681 Sec3.2.3
        c.m_cnwd = c.m_ssthresh + 3 * c.m_Smss;
    }
    // Fast Recovery stage
    else if (c.m_dupAmount > 3)
    {
        c.m_congPhase = TcpCongPhase::FAST_RECOVER;
        // RFC5681 Sec3.2.4
        c.m_cnwd += c.m_Smss;
    }
    // Slow Start Phase
    else if (c.m_cnwd < c.m_ssthresh)
    {
        c.m_congPhase = TcpCongPhase::SLOW_START;
        // RFC5681 Sec3.1 Equation (2)
        c.m_cnwd += std::min(c.m_N, c.m_Smss);
    }
    // Congestion Avoidence
    else
    {
        c.m_congPhase = TcpCongPhase::CONG_AVOID;
        // RFC5681 Sec3.1 Equation (3) and the paragraph after equation
        c.m_cnwd += std::max(c.m_Smss * c.m_Smss / c.m_cnwd, (uint32_t) 1);
    }

    return std::make_pair(result, c);
}

} // namespace ns3