#include "Geometry.h"

Path::Path(Point _origin, Point _end){
    origin = _origin;
    end = _end;
}

Path::~Path(){}

Trajectory::Trajectory(Point _origin) 
    : Path(_origin,_origin){}

Trajectory::Trajectory(Point _origin, Point _end)
    : Path(_origin,_end){}

Trajectory::~Trajectory(){
    for(int i = 0; i < path.size(); i++){
        path[i]->~Path();
    }
}

