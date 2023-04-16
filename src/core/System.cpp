#include "core/System.h"
#include <Arduino.h>



namespace TwinSystem{

        System::System(){}

        void System::Initialize(){
            ui.Initialize();
            actuators.Initialize();
            motion.Initialize();
            intercom.Initialize();
            
        }

        void System::Update(){
            PollEvents();
            //motion.UpdatePosition();
            intercom.Update();
            
            /*
            String command = "lookAt(";
            command += (motion.GetAbsPosition().c * RAD_TO_DEG);
            command +=  ",500);";

            intercom.SendCommand(command.c_str());

            if(intercom.IsSomethingAtAngle(motion.GetAbsPosition().c * RAD_TO_DEG)){
                motion.Pause();
            }else{
                if(motion.IsPaused()) motion.Resume();
            }
            */

            ui.Update();
            delay(200);
        }

        void System::OnEvent(Event& e){
            Console::trace("Event") << e.ToString() << Console::endl;
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

        void System::WaitUntil(JobExecutor& obj){
            while (obj.IsBusy()){
                obj.Run();
                
                Update();
            }
        }

        void System::PollEvents(){
            
        }


} // namespace TwinSystem