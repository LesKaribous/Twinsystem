#include "services/service.h"
#include "utils/timer/timer.h"
#include "settings.h"

using routine_ptr = void (*)();

class Chrono : public Service, public Timer{
public:

    void attach() override;
    void run() override;

    void onMatchNearlyFinished();
    void onMatchFinished();

    void setNearEndCallback(routine_ptr cb);
    void setEndCallback(routine_ptr cb);

    Chrono(): Service(ID_CHRONO){};
    SINGLETON(Chrono)

private :
    routine_ptr m_matchNearEnd_callback;
    routine_ptr m_matchEnd_callback;

    bool nearlyFinished = false;
    bool finished = false;
};
SINGLETON_EXTERN(Chrono, chrono)