#include "services/service.h"
#include "settings.h"

class Safety : public Service{
public:

    void onAttach()override;
    void onUpdate()override;

    void setObstacleDetected(bool state);
    void setSafeDistance(int safeDistance);

    Safety(): Service(ID_SAFETY){};
    SERVICE(Safety)

private:
    int m_currentDistance = infinity();
    bool m_obstacleDetected = false;
    int m_lastSeen = infinity();
};
EXTERN_DECLARATION(Safety, safety)