/**
  CommandManager.h
  4/28/2023
  © Camren Mumme 2023
**/

// Includes
#include "StaticSerialCommands.h"
#include "EEPROMAddresses.h"
#include "AltimeterManager.h"
#include "EEPROM.h"

namespace CommandManager {
    // Help command
    void Help(SerialCommands& Sender, Args& _) {
        Sender.listCommands();
    }

    // Proceed command
    bool ProceedListener = false;
    bool ProceedTrigger = false;

    void Proceed(SerialCommands& Sender, Args& _) {
        if(!ProceedListener) return;

        ProceedTrigger = true;
    }

    void AwaitProceedCommand() {
        ProceedListener = true;
        while(!ProceedTrigger) { }
        ProceedListener = false;
    }

    // Barometer command
    void SetBarometer(SerialCommands& Sender, Args& Arguments) {
        double NewBarometerSetting = Arguments[0].getFloat();

        EEPROM.put(EEPROMAddress::BarometerCalibration, NewBarometerSetting);
        AltimeterManager::BarometerCalibration = NewBarometerSetting;
    }

    Command Commands[] {
        COMMAND(Help, "help"),
        COMMAND(Proceed, "proceed", nullptr, "Proceeds with initialization"),
        COMMAND(SetBarometer, "barometer", ArgType::Float, nullptr, "Sets the barometer calibration")
    };
    SerialCommands CommandDaemon(Serial, Commands, sizeof(Commands) / sizeof(Command));

    void Update() {
        CommandDaemon.readSerial();
    }
}