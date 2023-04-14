#include "Inputs.h"
#include "Pin.h"
#include "event/Event.h"
#include "event/InputEvent.h"

namespace TwinSystem{

    Inputs::Inputs() : 
        resetButton(Pin::Inputs::resetButton),
        teamSwitch(Pin::Inputs::teamSwitch),
        strategySwitch(Pin::Inputs::strategySwitch),
        twinSwitch(Pin::Inputs::twinSwitch),
        starter(Pin::Inputs::starter)
    {}


    void Inputs::Initialize(){
        resetButton.init();
        teamSwitch.init();
        starter.init();
        strategySwitch.init();
        twinSwitch.init();
    }

    void Inputs::Update(){
        resetButton.update();
        teamSwitch.update();
        starter.update();
        strategySwitch.update();
        twinSwitch.update();
    }

    bool Inputs::HasChanged(){
        return resetButton.hasChanged() || starter.hasChanged() || teamSwitch.hasChanged() || strategySwitch.hasChanged() || twinSwitch.hasChanged();
    }


    Inputs::Inputs() : 
        resetButton(Pin::Inputs::resetButton),
        teamSwitch(Pin::Inputs::teamSwitch),
        strategySwitch(Pin::Inputs::strategySwitch),
        twinSwitch(Pin::Inputs::twinSwitch),
        starter(Pin::Inputs::starter)
    {}


    void References::Initialize(){

    }

    void References::Update(){

    }

    bool References::HasChanged(){ //TODO Add the rest
        return x.hasChanged() || y.hasChanged() || z.hasChanged() || team.hasChanged() || avoidance.hasChanged();
    }

    /*
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
    */


    
} // namespace TwinSystem
