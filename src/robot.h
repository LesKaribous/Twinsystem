
#include "os/os.h"
#include "os/asyncExecutor.h"
#include "os/console.h"
#include "services/ihm/ihm.h"
#include "services/motion/motion.h"
#include "services/lidar/lidar.h"
#include "services/intercom/intercom.h"
#include "services/actuators/actuators.h"
#include "services/terminal/terminal.h"
#include "utils/interpreter/interpreter.h"

extern OS& os;
extern IHM& ihm;
extern Motion& motion;
extern Actuators& actuators;
extern Intercom& intercom;
extern Terminal& terminal;
extern Lidar& lidar;

