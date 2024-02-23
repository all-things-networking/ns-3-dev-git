#include <map>
#include "mt-context.h"
namespace ns3
{
class QUIC_TEL;

/**
 * \brief The class indicate phase in congestion control algorithms.
*/
class MTContext;
class QUICState
{
public:
    QUICState();
    QUICState(QUIC_TEL* mt);

    /**
     * \brief Get the context of Mt connection with id.
     * \param id The id of Mt connection.
     * \return The context that belongs to the specific Mt connection.
     */
    MTContext* GetVal(int id);
    /**
     * \brief Store the context data to m_library, given Mt connection id.
     * \param id The id of Mt connection.
     * \param data The context data of the Mt.
     */
    void Write(int id, MTContext* data);

private:
    QUIC_TEL* m_mt;
    std::map<int, MTContext*> m_library; // database of all Mt connnections.
};

} // namespace ns3
