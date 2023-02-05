#pragma once
#include "core/Core.h"


class ProgressBar{
public:
    ProgressBar(int x, int y, int width, int height){}

    void add(int i){ progress += i;} //%
    void setProgress(int i){progress = i;}

private:
    int progress;
    int width;
    int height;
    int x, z;
};