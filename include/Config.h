#pragma once
#include <stdint.h>

namespace Config{
    
    namespace Speed{
        const uint32_t defaultSpeed = 20000;
        const uint32_t defaultAccel = 40000;
    }

    namespace Calibration{
        namespace Primary{
            const float linearFactor = 33.3;
            const float rotationalFactor = 52.9;
            const float leftStepperFactor = 32.0;
            const float rightStepperFactor = 32.0;
        }
        namespace Secondary{
            const float linearFactor = 8.0;
            const float rotationalFactor = 14.0;
            const float leftStepperFactor = 8.0;
            const float rightStepperFactor = 8.0;
        }
    };
}
