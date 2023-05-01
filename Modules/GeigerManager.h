/**
  GeigerManager.h
  4/28/2023
  © Camren Mumme 2023
**/
#pragma once

// Includes
#include "RadiationWatch.h"

namespace GeigerManager {
    RadiationWatch ControlGeiger(2,3);
    RadiationWatch BariumGeiger(4,5);
    RadiationWatch BismuthGeiger(6,7);

    void SetupGeiger(RadiationWatch* Geiger, arduino::String FileName, void (*RadiationCallback)(void), arduino::String DisplayName) {
        Geiger->setup();
        Geiger->registerRadiationCallback(RadiationCallback);
        LoggingManager::Log(DisplayName+" setup with logfile "+FileName);
    }

    void Start() {
        ControlGeiger.setup();
        BariumGeiger.setup();
        BismuthGeiger.setup();
    }

    void Update() {
        ControlGeiger.loop();
        BariumGeiger.loop();
        BismuthGeiger.loop();
    }
}