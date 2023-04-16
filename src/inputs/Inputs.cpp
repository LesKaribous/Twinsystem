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

        resetButton.SetInverted(true);
        teamSwitch.SetInverted(true);
        starter.SetInverted(true);
        strategySwitch.SetInverted(true);
        twinSwitch.SetInverted(true);

        resetButton.Init();
        teamSwitch.Init();
        starter.Init();
        strategySwitch.Init();
        twinSwitch.Init();
    }

    void Inputs::Read(){
        resetButton.Read();
        teamSwitch.Read();
        starter.Read();
        strategySwitch.Read();
        twinSwitch.Read();
    }

    bool Inputs::HasChanged(){
        return resetButton.HasChanged() || starter.HasChanged() || teamSwitch.HasChanged() || strategySwitch.HasChanged() || twinSwitch.HasChanged();
    }

    void Inputs::OnEvent(Event& e){
        
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
