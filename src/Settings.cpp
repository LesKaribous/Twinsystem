#include "Settings.h"
#include "Pin.h"
#include "Actuators.h"
#include "Twinsystem.h"

namespace Settings{

    bool 
    ROBOT = true,
    AVOIDANCE = true,
    TEAM = Settings::YELLOW;

    Vec3 transform; //Changement de reprère purple / yellow 

    void init(){ 
        pinMode(Pin::robotSelect, INPUT_PULLUP);
        ROBOT = digitalRead(Pin::robotSelect) ? PRIMARY : SECONDARY;
        IHM::setTeam(TEAM);

        IHM::addLoad(10);
        IHM::setLoadingMsg("Settings OK");
    }

    void setTeam(bool team){
        TEAM = team;
    }

    void setAvoidance(bool state){
        AVOIDANCE = state == Settings::ADVERSAIRE_OUI;
    }

    bool yellow     (){ return (Settings::TEAM == Settings::YELLOW);        }
	bool purple     (){ return (Settings::TEAM == Settings::PURPLE);        }
    bool primary    (){ return (Settings::ROBOT == Settings::PRIMARY);      }
	bool secondary  (){ return (Settings::ROBOT == Settings::SECONDARY);    }
    bool avoidance  (){ return Settings::AVOIDANCE;                         }
    bool useIHM     (){ return Settings::IHM;                               }

}

