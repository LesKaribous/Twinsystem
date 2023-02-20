#include "Inputs.h"
#include "Pin.h"
#include "event/Event.h"
#include "event/InputEvent.h"

namespace TwinSystem{

    Inputs::Inputs() : 
    init(Pin::Inputs::init),
    team(Pin::Inputs::team),
    starter(Pin::Inputs::starter),
    strategy(Pin::Inputs::strategy){}

    bool Inputs::pollEvents(std::function<void(Event&)> cb){

        init.update();
        team.update();
        starter.update();
        strategy.update();

        if(init.hasChanged()){
            InputEvent e("Init", starter.GetState());
            cb(e);
        }

        if(team.hasChanged()){
            InputEvent e("Team", starter.GetState());
            cb(e);
        }

        if(starter.hasChanged()){
            InputEvent e("Starter", starter.GetState());
            cb(e);
        }

        if(strategy.hasChanged()){
            InputEvent e("Strategy", starter.GetState());
            cb(e);
        }
    }
    


    
} // namespace TwinSystem
