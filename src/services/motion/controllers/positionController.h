#pragma once
#include "ControllerBase.h"

class PositionController : public ControllerBase {
    public:
        PositionController();
        
        void setTargetPositions(std::vector<long> positions);
        void control();
        
        bool isArrived() const;
    };