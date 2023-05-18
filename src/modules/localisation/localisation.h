#include "core/module.h"
#include "core/lib.h"
#include "math/geometry.h"

class Localisation : public Module{

public:
    Localisation();

    void update() override;

    void resetDrift();
    Vec2 getTotalDrift();
    Vec3 accumulateDrift(Vec3 estimateCartesian);

private:
    unsigned int _samples = 10;
    Vec2 _driftAccumulator;
    Vec3 _measureRaw;
    std::deque<Vec3> _samplesMeasure;
};