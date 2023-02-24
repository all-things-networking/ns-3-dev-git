#ifndef QUIC_CONGESTION_CONTROLLER_H
#define QUIC_CONGESTION_CONTROLLER_H
#include "QUIC-Dispatcher.h"
#include "QUIC-EventProcessor.h"

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

namespace ns3
{
class MTEvent;
class MTEventQueue;
class MTHeader;

enum TcpCongestionControlAlgorithm {
  TCPNewReno,
  Cubic
};

/**
 * \brief The processor handling the congestion control events.
 */
class QUICCongestionController: public QUICEventProcessor
{
private:
    int windowSize;
    TcpCongestionControlAlgorithm congestionControlAlgorithm = TCPNewReno; // Default value is TCPNewReno
public:
    QUICCongestionController();
    EventProcessorOutput* Process(MTEvent e, MTContext* c);
    bool IsValidEvent(MTEvent e);
    void SetWindowSize(int value);
    int GetWindowSize();
    void SetCongestionControlAlgorithm(TcpCongestionControlAlgorithm algorithm);
};
} // namespace ns3
#endif
