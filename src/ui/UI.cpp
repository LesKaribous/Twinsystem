#include "UI.h"
#include "Pin.h"
#include "core/System.h"

namespace TwinSystem{

    using namespace Pin;

    UI::UI() : 
    screen(TFT::CS, TFT::DC, TFT::RST, TFT::MOSI, TFT::SCK, TFT::MISO)
    {
        screen.begin();
        clear();
        needDraw = true;
    }
    
    /*
    bool UI::pollEvents(std::function<void(Event&)> cb){
        inputs.pollEvents(cb);
    }
    */
    void UI::OnEvent(Event& e){
        
        if(e.IsInCategory(EventCategory::EventCategoryUI)){
            needDraw = true;
        }

    }
    
    void UI::clear(){
        screen.fillScreen(ILI9341_BLACK);
        screen.setTextSize(2);
    }

    void UI::draw(){
        
        if(millis() - lastDraw > 200) needDraw = true;
        if(!needDraw) return;
        needDraw = false;

        lastDraw = millis();
        screen.setTextColor(ILI9341_YELLOW);

        screen.fillRect(0,0,200,200, ILI9341_BLACK);


        screen.drawString("Init : ",0,0);       screen.drawNumber(digitalRead(Pin::Inputs::init),150,0);
        screen.drawString("Tirette : ",0,20);   screen.drawNumber(digitalRead(Pin::Inputs::starter),150,20);
        screen.drawString("Strategie : ",0,40); screen.drawNumber(digitalRead(Pin::Inputs::strategy),150,40);
        
        System s = System::GetInstance();

        Vec3 cPos = s.motion->GetPosition();

        screen.drawString("Position : ", 0, 80); 
        cPos.a == -1 ? screen.drawString("?", 130, 80) : screen.drawNumber(cPos.a ,130,80); 
        cPos.b == -1 ? screen.drawString("?", 130, 100) : screen.drawNumber(cPos.b ,130,100); 
        screen.drawNumber(cPos.c ,130,120);

        bool team_color = digitalRead(Pin::Inputs::team);
        if (team_color) screen.setTextColor(ILI9341_BLUE);
        else screen.setTextColor(ILI9341_GREEN);
        screen.drawString("Couleur : ",0,60);   screen.drawNumber(team_color,150,60);
        
    }
} // namespace TwinSystem
