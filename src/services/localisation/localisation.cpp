#include "localisation.h"
#include "os/console.h"

#include <Wire.h>
#include <SPI.h>

INSTANTIATE_SERVICE(Localisation, localisation);

#ifdef OLD_BOARD
#define I2C_OTOS I2C_OTOS
#else
#define I2C_OTOS Wire
#endif

void Localisation::onAttach(){
    
    Console::info() << "Localisation activated" << Console::endl;
    I2C_OTOS.begin();

    m_connected = false;
    for(int i = 0; i < 10; i++){
        if(otos.begin(I2C_OTOS) == true){
            m_connected = true;           
            break;
        }
    }

    if(!m_connected){
        Console::error("Localisation") << "Ooops, no OTOS detected ... It may be unplugged. Make sure you use I2C_OTOS" << Console::endl;
    }
    else{
        otos.setLinearUnit(kSfeOtosLinearUnitMeters);
        otos.setAngularUnit(kSfeOtosAngularUnitRadians);
        otos.resetTracking();
        Console::success("Localisation") << "OTOS connected" << Console::endl;
    }
}

// Main loop
void Localisation::onUpdate(){ 
    //THROW(1)

    static long elapsed = 0;
    if(millis() - elapsed > m_refresh){
        elapsed = millis();
        read();
    }
}

// Service routines
void Localisation::enable(){
    if(!m_connected) return;
    Service::enable();
    //servicethread = new std::thread(&Localisation::runThread, this);
 	//servicethread->detach();
    m_use_IMU = true;
}

void Localisation::disable(){
    Service::disable();
    //delete servicethread;
    m_use_IMU = false;
}

void Localisation::setPosition(Vec3 newPos){
    sfe_otos_pose2d_t pos;
    pos.x = -newPos.y/1000.0;
    pos.y = -newPos.x/1000.0;
    pos.h = newPos.z;
    otos.setPosition(pos);
    _unsafePosition = newPos;
}

Vec3 Localisation::getPosition(){
    return _unsafePosition;
}

void Localisation::read()
{
    sfe_otos_pose2d_t myPosition;
    otos.getPosition(myPosition);

    //Console::info() << "x:" << myPosition.x << "y:" << myPosition.y << "h:" << myPosition.h << Console::endl;

    _unsafePosition.x = -myPosition.y * 1000.0; //to millimeters
    _unsafePosition.y = -myPosition.x * 1000.0; //to millimeters


    float orientation = myPosition.h;
    _unsafePosition.z = orientation;
    //Console::info() << _unsafePosition << Console::endl;
}

void Localisation::calibrate() {
    Serial.println("Ensure the OTOS is flat and stationary");
    delay(1000);
    Serial.println("Calibrating IMU...");

    // Calibrate the IMU, which removes the accelerometer and gyroscope offsets
    otos.calibrateImu();
    Serial.println("Calibrated IMU.");
    m_calibrated = true;
}
