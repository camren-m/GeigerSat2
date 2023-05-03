/**
  AltimeterManager.cpp
  4/28/2023
  © Camren Mumme 2023
**/

// Includes
#include <EEPROM.h>
#include "EEPROMAddresses.h"
#include "LoggingManager.h"
#include "AltimeterManager.h"

namespace AltimeterManager {
    void LoadCalibrationFromEEPROM() {
        double SavedBarometerCalibration;
        EEPROM.get(EEPROMAddress::BarometerCalibration, SavedBarometerCalibration);
        LoggingManager::Log("Found altimeter calibration value of "+arduino::String(SavedBarometerCalibration)+" in EEPROM.");
        if(SavedBarometerCalibration == 0xFF) return;
        BarometerCalibration = SavedBarometerCalibration;
    }

    float Altitude() {
        Barometer.performReading();
        return Barometer.readAltitude(BarometerCalibration);
    }

    void Start() {
        LoadCalibrationFromEEPROM();
        if(!Barometer.begin_I2C()) {
            LoggingManager::Log("Could not initialize barometer I2C. Aborting.");
            while(1) { }
        }

        Barometer.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        Barometer.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        Barometer.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
        Barometer.setOutputDataRate(BMP3_ODR_50_HZ);

        LoggingManager::Log("Altimeter initialized.");
    }
}