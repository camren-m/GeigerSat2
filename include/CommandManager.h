/**
  CommandManager.h
  4/28/2023
  © Camren Mumme 2023
**/

#pragma once

// Includes
#include <StaticSerialCommands.h>

namespace CommandManager {
    static bool InitializeCommandRun = false;

    void Help(SerialCommands& Sender, Args& _);
    void Initialize(SerialCommands& Sender, Args& _);
    void SetBarometer(SerialCommands& _, Args& __);
    void GetBarometer(SerialCommands& _, Args& __);
    void EraseEEPROM(SerialCommands& _, Args& __);
    void Reset(SerialCommands& _, Args& __);
    void GetMemory(SerialCommands& _, Args& __);
    void ReadEEPROM(SerialCommands& _, Args& __);

    String convert_to_hex_str(int decimal);

    static Command Commands[] {
        COMMAND(Help, "help"),
        COMMAND(Initialize, "init", nullptr, "Proceeds with GeigerSat2 initialization."),
        COMMAND(SetBarometer, "brset", ArgType::Float, nullptr, "Sets the barometer calibration in inHg."),
        COMMAND(GetBarometer, "brstat", nullptr, "Fetches the current barometer status."),
        COMMAND(EraseEEPROM, "erase", nullptr, "Erases all values stored in the EEPROM."),
        COMMAND(ReadEEPROM, "read", nullptr, "Fetches all values stored in the EEPROM and displays them."),
        COMMAND(GetMemory, "mem", nullptr, "Returns the currently available amount of memory."),
        COMMAND(Reset, "reset", nullptr, "Resets GeigerSat2.")
    };
    static SerialCommands CommandDaemon(Serial, Commands, sizeof(Commands) / sizeof(Command));


    void Update();
}