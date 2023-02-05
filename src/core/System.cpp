#include "core/System.h"
#include <Arduino.h>

namespace TwinSystem{

        System* System::s_Instance = nullptr;
        
        System::System(){
            if(s_Instance == nullptr){
                s_Instance = this;
            }else{
               Console::error("System") << "System instance already created." << Console::endl;
            }
        }

        void System::Run(){
            pollEvents();
        }

        void System::Initialize(){
            
        }

        void System::OnEvent(Event& e){
            
        }

        void System::Execute(Program prgm){
            
        }

        void System::wait(int temps){
            unsigned long initTemps = millis();
            while ((millis() - initTemps) <= temps){
                Run();
            }
        }

        void System::pollEvents(){
            
        }


} // namespace TwinSystem