/**
  CommandManager.cpp
  4/28/2023
  © Camren Mumme 2023
**/

// Includes
#include <EEPROM.h>
#include <StaticSerialCommands.h>
#include <MemoryFree.h>
#include "EEPROMAddresses.h"
#include "AltimeterManager.h"
#include "LoggingManager.h"
#include "ResetManager.h"
#include "CommandManager.h"

namespace CommandManager {
    // Help command
    void Help(SerialCommands& Sender, Args& _) {
        Sender.listCommands();
    }

    void Initialize(SerialCommands& Sender, Args& _) {
        InitializeCommandRun = true;
    }

    // Barometer update command
    void SetBarometer(SerialCommands& Sender, Args& Arguments) {
        double NewBarometerSettinginHg = Arguments[0].getFloat();
        double hPaConversion = NewBarometerSettinginHg * 33.8639;

        EEPROM.put(EEPROMAddress::BarometerCalibration, hPaConversion);
        AltimeterManager::BarometerCalibration = hPaConversion;
        LoggingManager::Log("Updated barometer calibration and EEPROM record via to "+arduino::String(hPaConversion)+" hPa/"+arduino::String(NewBarometerSettinginHg)+" inHg via the baro-set command.");
    }

    // Barometer get command
    void GetBarometer(SerialCommands& Sender, Args& _) {
        Serial.println(AltimeterManager::BarometerCalibration);
    }

    // Erase EEPROM command
    void EraseEEPROM(SerialCommands& Sender, Args& _) {
        LoggingManager::Log("Erasing EEPROM...");
        for (int i = 0; i < EEPROM.length(); i++) {
            EEPROM.write(i, 0);
        }
        LoggingManager::Log("EEPROM erased.");
    }

    // Read EEPROM command
    void ReadEEPROM(SerialCommands& _, Args& __) {
        LoggingManager::Log("Reading entire EEPROM...");
        LoggingManager::Log("EEPROM size: "+String(EEPROM.length())+" bytes");

        for (int i = 0; i < EEPROM.length(); i++) {
            Serial.print(EEPROM.read(i));
        }
        Serial.print("\n");

        LoggingManager::Log("Read entire EEPROM.");
    }

    // Reset command
    void Reset(SerialCommands& Sender, Args& _) {
        ResetManager::Reset();
    }

    void GetMemory(SerialCommands& _, Args& __) {
        Serial.println(FreeRam());
    }

    void Update() {
        CommandDaemon.readSerial();
    }
}