#include "modules/module.h"
#include "system/core/lib.h"
#include "system/math/geometry.h"

class Localisation : public Module{
public:
    Localisation();

    void update() override;
    void addMeasure(const Vec3&);
    void resetDrift();

    Vec2 estimateDrift(Vec3 estimateCartesian);

private:
    int _samples = 10;
    Vec3 _measureRaw;
    std::deque<Vec3> _samplesMeasure;
};