/**
  main.cpp
  4/28/2023
  © Camren Mumme 2023
**/

// Preprocessor Directives
#define LIBCALL_RADIATIONWATCH // prevents multiple definition of RadiationWatch::setupInterrupt()

// Includes
#include "LoggingManager.h"
#include "CommandManager.h"
#include "AltimeterManager.h"
#include "GeigerManager.h"

namespace GeigerSat2 {
    bool Initialized = false;

    void Initialize() {
        Serial.println(F("Proceeding with initialization."));
        LoggingManager::Start();
        LoggingManager::Log("Hello! Starting our tests and subsystem initializations now.");
        AltimeterManager::Start();
        GeigerManager::Start();

        LoggingManager::Log("GeigerSat2 initialized. Welcome!");
        LoggingManager::Log("   Copyright (c) 2023 Camren Mumme");

        Initialized = true;
    }
}

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println(F("GeigerSat2 ready for initialization. Send init command to initialize."));
    Serial.println(F("------------------------------------------------------------------------"));
}

void loop() {
    CommandManager::Update();

    if(CommandManager::InitializeCommandRun && !GeigerSat2::Initialized) {
        GeigerSat2::Initialize();
    }
    if(!GeigerSat2::Initialized) return;
    GeigerManager::Update();
    LoggingManager::Update();
}