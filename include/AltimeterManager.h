/**
  AltimeterManager.h
  4/28/2023
  © Camren Mumme 2023
**/

#pragma once

// Includes
#include "Adafruit_BMP3XX.h"

namespace AltimeterManager {
    static double BarometerCalibration = 1013.25;
    static Adafruit_BMP3XX Barometer;

    void Start();
}