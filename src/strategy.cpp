#include "extension/strategy/extStrategy.h"
#include "system.h"


using namespace POI;
using namespace Score;

ExtStrategy::ExtStrategy(System& sys) : system(sys), 
motion(*sys.motion), actuators(*sys.actuators){

}

ExtStrategy::~ExtStrategy(){
}

