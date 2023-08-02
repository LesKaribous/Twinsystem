#include "localisation.h"
#include "system/core/lib.h"
#include "os.h"
#include "pin.h"

Localisation::Localisation() : Service(LOCALISATION){
}

void Localisation::update(){
    if (m_enabled){


    }
}

void Localisation::addMeasure(const Vec3& m){
    if (m_enabled){

        if(m.a < 0 || m.b < 0 || m.c < 0) return;

        _samplesMeasure.push_front(m);
        
        if(_samplesMeasure.size() >= _samples){
            _samplesMeasure.pop_back();
            Vec3 sum = Vec3(0.0);
            for(const Vec3& v : _samplesMeasure)
                sum += v;

            sum /= float(_samples);
            _measureRaw = sum;
        }
    }
}


Vec2 Localisation::estimateDrift(Vec3 estimateCartesian){
    if (m_enabled){

        Vec3 estimated;
        estimated.x = getBorderDistance(Vec2(estimateCartesian), estimateCartesian.c);
        estimated.y = getBorderDistance(Vec2(estimateCartesian), estimateCartesian.c + 120*DEG_TO_RAD);
        estimated.z = getBorderDistance(Vec2(estimateCartesian), estimateCartesian.c - 120*DEG_TO_RAD);
        
        os.console.info("Estimated") << Vec3(estimateCartesian.c *RAD_TO_DEG , estimateCartesian.c*RAD_TO_DEG + 120, estimateCartesian.c*RAD_TO_DEG + 120) << os.console.endl;
        os.console.info("Estimated") << estimated << os.console.endl;
        os.console.info("Mesured") << _measureRaw << os.console.endl;

        Vec3 polarDrift = estimated - _measureRaw;

        PolarVec dA(estimateCartesian.c, polarDrift.a);                     Vec2 cdA = dA.toVec2();
        PolarVec dB(estimateCartesian.c + 120*DEG_TO_RAD, polarDrift.b);    Vec2 cdB = dB.toVec2();
        PolarVec dC(estimateCartesian.c - 120*DEG_TO_RAD, polarDrift.c);    Vec2 cdC = dC.toVec2();

        return (cdA + cdB + cdC) / 3.0;
    }
}



