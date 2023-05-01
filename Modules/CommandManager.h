/**
  CommandManager.h
  4/28/2023
  © Camren Mumme 2023
**/

// Includes
#include "StaticSerialCommands.h"
#include "EEPROMAddresses.h"
#include "AltimeterManager.h"
#include "ResetManager.h"
#include "EEPROM.h"

namespace CommandManager {
    // Help command
    void Help(SerialCommands& Sender, Args& _) {
        Sender.listCommands();
    }

    // Proceed command
    bool InitializeCommandRun = false;

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

    // Reset command
    void Reset(SerialCommands& Sender, Args& _) {
        ResetManager::Reset();
    }

    Command Commands[] {
        COMMAND(Help, "help"),
        COMMAND(Initialize, "init", nullptr, "Proceeds with GeigerSat2 initialization."),
        COMMAND(SetBarometer, "baro-set", ArgType::Float, nullptr, "Sets the barometer calibration in inHg."),
        COMMAND(GetBarometer, "baro-status", nullptr, "Fetches the current barometer status."),
        COMMAND(EraseEEPROM, "erase-eeprom", nullptr, "Erases all values stored in the EEPROM.")
    };
    SerialCommands CommandDaemon(Serial, Commands, sizeof(Commands) / sizeof(Command));

    void Update() {
        CommandDaemon.readSerial();
    }
}