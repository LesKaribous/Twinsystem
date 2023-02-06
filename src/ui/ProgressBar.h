#pragma once
#include "core/Core.h"
#include "Screen.h"

class ProgressBar{
public:
    ProgressBar() :
     _progress(0), _width(100), _height(20), _x(10), _y(10){}

    void add(int i){ _progress += i;} //%
    void setProgress(int i){ _progress = i;}
    void setMessage(std::string msg){}

    void setPosition(int x, int y);
    void setDimension(int w, int h);

    void draw(Screen& screen){
        
    }

private:
    int _progress;
    int _width;
    int _height;
    int _x, _y;
    std::string _msg;
};