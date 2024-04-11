#include "services/service.h"
#include "settings.h"

class Safety : public Service{
public:

    void onAttach()override;
    void onUpdate()override;

    Safety(): Service(ID_SAFETY){};
    SERVICE(Safety)

private:

};
EXTERN_DECLARATION(Safety, safety)