#include "core/System.h"
#include <Arduino.h>

namespace TwinSystem{

        System* System::s_Instance = nullptr;
        
        System::System(){
            if(s_Instance == nullptr){
                s_Instance = this;
                Initialize();
            }else{
               Console::error("System") << "System instance already created." << Console::endl;
            }
        }

        void System::Run(){
            //pollEvents();
            ui.inputs.update();
            ui.draw();
        }

        void System::Initialize(){
            Console::Initialize();
            ui.inputs.initialize();
            Console::SetLevel(ConsoleLevel::_TRACE);
            stepper = CreateShared<StepperController>();
            motion = CreateShared<MotionControl>(stepper);
        }

        void System::OnEvent(Event& e){
            //EventDispatcher dispatcher(e);
            //dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
            //dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));

            ui.OnEvent(e);
            motion->OnEvent(e);
        }

        void System::Execute(Program prgm){
            
        }

        void System::wait(int temps){
            unsigned long initTemps = millis();
            while ((millis() - initTemps) <= temps){
                Run();
            }
        }

        bool System::pollEvents(){
            //ui->pollEvents(&System::OnEvent);
        }


} // namespace TwinSystem