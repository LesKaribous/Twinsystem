#include "core/System.h"
#include <Arduino.h>

namespace TwinSystem{

        System::System(){}


        void System::Initialize(){
            if(!_initialized){
                _initialized = true;
                
                ui.Initialize();
                actuators.Initialize();
                motion.Initialize();
                intercom.Initialize();
            
            }
        }

        void System::Update(){
            //pollEvents();
            ui.Update();
            intercom.Update();
        }



        void System::OnEvent(Event& e){
            //EventDispatcher dispatcher(e);
            //dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
            //dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));

            ui.OnEvent(e);
            motion.OnEvent(e);
        }

        void System::Execute(Program prgm){
            
        }

        void System::Wait(int temps){
            unsigned long initTemps = millis();
            while ((millis() - initTemps) <= temps){
                Update();
            }
        }

        bool System::pollEvents(){
            //ui->pollEvents(&System::OnEvent);
        }


} // namespace TwinSystem