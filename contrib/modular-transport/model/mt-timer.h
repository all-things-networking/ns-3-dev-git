#ifndef MT_TIMER
#define MT_TIMER

/**
 * Reference:
 * https://coe.northeastern.edu/Research/krclab/crens3-doc/classns3_1_1_timer.html#a9ecd4720dd3c082dab33f2d1e066814c
 * https://www.nsnam.org/docs/release/3.19/doxygen/classns3_1_1_time.html#aee3397d2096a21d6aa69a7dae4484614
 */

namespace ns3
{

class Timer;
class ModularTransport;

/**
 * \brief Timer class that will trigger function calls after timeout
 * 
 * Timer class is based on ns3::Timer Class.
 * Unit of the timer is milliseconds.
 */
class MtTimer
{
public:
    MtTimer(ModularTransport* mt, uint32_t duration, int tcpId);

    /**
     * \brief Set the timer duration in milliseconds. This will also reset timer.
     * \param duration New time duration.
     */
    void SetDuration(uint32_t duration);

    /**
     * \brief Get the timer duration.
     * \return Time duration in milliseconds.
     */
    uint32_t GetDuration();
  
    /**
     * \brief Start the timer or resume the stopped timer.
     *
     * Calling Start on runnning timer or timer that expired have no effects.
     */
    void Start();

    /**
     * \brief Stop the timer
     */
    void Stop();

    /**
     * \brief Reset the timer, need to call Start() again.
     */
    void Reset();

    /**
     * \brief Check if timer is expired/triggered.
     */
    void IsFinished();

private:
    int m_tcpId;            // Id of the tcp connnectoin.
    uint32_t m_duration;    // Timer duration before timeout.
    Timer m_ns3Timer;       // Ns3 class managing the timer.
    ModularTransport* m_mt; // ModularTransport Instance.
};

}
#endif