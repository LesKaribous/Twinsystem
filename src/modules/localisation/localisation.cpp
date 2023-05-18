#include "modules/localisation/localisation.h"
#include <algorithm>

Localisation::Localisation() : Module(LOCALISATION){
    _driftAccumulator = Vec2(0.0f, 0.0f);
}


void Localisation::update(){
    if (m_enabled){

        //Read
        _samplesMeasure.push_front(Vec3(0.0));
        
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

void Localisation::resetDrift(){
    if (m_enabled)
        _driftAccumulator = Vec2(0.0f, 0.0f);
}

Vec2 Localisation::getTotalDrift(){
    if (m_enabled)
        return _driftAccumulator;
    else return Vec2(0.0);
}

Vec3 Localisation::accumulateDrift(Vec3 estimateCartesian){
    if (m_enabled){

        Vec3 estimated;
        estimated.x = getBorderDistance(Vec2(estimateCartesian), estimateCartesian.c);
        estimated.y = getBorderDistance(Vec2(estimateCartesian), estimateCartesian.c + 120);
        estimated.z = getBorderDistance(Vec2(estimateCartesian), estimateCartesian.c - 120);
        
        Vec3 polarDrift = estimated - _measureRaw;

        PolarVec dA(estimateCartesian.c, polarDrift.a);       Vec2 cdA = dA.toVec2();
        PolarVec dB(estimateCartesian.c + 120, polarDrift.b); Vec2 cdB = dB.toVec2();
        PolarVec dC(estimateCartesian.c - 120, polarDrift.c); Vec2 cdC = dC.toVec2();

        _driftAccumulator += (cdA + cdB + cdC) / 3.0;
    }
}



