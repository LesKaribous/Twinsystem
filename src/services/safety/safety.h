#include "services/service.h"
#include "settings.h"

class Safety : public Service{
public:

    void onAttach()override;
    void onUpdate()override;

    void setSafeDistance(int safeDistance);

    Safety(): Service(ID_SAFETY){};
    SERVICE(Safety)

private:
    int m_currentDistance;
};
EXTERN_DECLARATION(Safety, safety)