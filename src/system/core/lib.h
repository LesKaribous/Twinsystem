#pragma once

#include <Arduino.h>
#include <Servo.h> //Actuators

//System
#include <map>
#include <array>
#include <deque>
#include <memory>
#include <vector>
#include <cstdint>
#include <stdint.h>
#include <algorithm>

#include <ILI9341_t3n.h> // Hardware-specific library for TFT Screen
#include <TeensyStep.h> //Motion

//Motion sensors
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>