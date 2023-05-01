/**
  AltimeterManager.h
  4/28/2023
  © Camren Mumme 2023
**/
#pragma once

// Includes
#include "EEPROM.h"
#include "EEPROMAddresses.h"
#include "LoggingManager.h"

namespace AltimeterManager {
    double BarometerCalibration = 1013.25;

    void LoadCalibrationFromEEPROM() {
        double SavedBarometerCalibration;
        EEPROM.get(EEPROMAddress::BarometerCalibration, SavedBarometerCalibration);
        LoggingManager::Log("Found altimeter calibration value of "+arduino::String(SavedBarometerCalibration)+" in EEPROM.");
        if(SavedBarometerCalibration == 0xFF) return;
        BarometerCalibration = SavedBarometerCalibration;
    }

    void Start() {
        LoadCalibrationFromEEPROM();
    }
}