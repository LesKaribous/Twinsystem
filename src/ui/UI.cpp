#include "UI.h"
#include "Pin.h"

namespace TwinSystem{

    using namespace Pin;

    UI::UI() : 
    screen(TFT::CS, TFT::DC, TFT::RST, TFT::MOSI, TFT::SCK, TFT::MISO)
    {
        screen.begin();
        clear();
        needDraw = true;
    }
    
    bool UI::pollEvents(std::function<void(Event&)> cb){
        inputs.pollEvents(cb);
    }

    void UI::OnEvent(Event& e){
        
        if(e.IsInCategory(EventCategory::EventCategoryUI)){
            needDraw = true;
        }

    }
    
    void UI::clear(){
        screen.fillScreen(ILI9341_BLACK);
    }

    void UI::draw(){
        if(!needDraw) return;
        screen.setTextColor(ILI9341_YELLOW);

        screen.fillRect(150,0,20,20, ILI9341_BLACK);
        screen.fillRect(150,20,20,20, ILI9341_BLACK);
        screen.fillRect(150,40,20,20, ILI9341_BLACK);
        screen.fillRect(150,60,20,20, ILI9341_BLACK);

        screen.drawString("Init : ",0,0);       screen.drawNumber(digitalRead(Pin::Inputs::init),150,0);
        screen.drawString("Tirette : ",0,20);   screen.drawNumber(digitalRead(Pin::Inputs::starter),150,20);
        screen.drawString("Strategie : ",0,40); screen.drawNumber(digitalRead(Pin::Inputs::team),150,40);

        bool team_color = digitalRead(Pin::Inputs::team);
        if (team_color) screen.setTextColor(ILI9341_BLUE);
        else screen.setTextColor(ILI9341_GREEN);
        screen.drawString("Couleur : ",0,60);   screen.drawNumber(team_color,150,60);
        needDraw = false;
    }
} // namespace TwinSystem
