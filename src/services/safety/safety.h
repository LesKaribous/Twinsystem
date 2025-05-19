#include "services/service.h"
#include "settings.h"

class Safety : public Service{
public:

    void enable() override;
    void disable() override;

    void attach()override;
    void run()override;

    void setObstacleDetected(bool state);
    void setSafeDistance(int safeDistance);

    Safety(): Service(ID_SAFETY){};
    SINGLETON(Safety)

private:
    int m_currentDistance = infinity();
    bool m_obstacleDetected = false;
    int m_lastSeen = infinity();
};
SINGLETON_EXTERN(Safety, safety)