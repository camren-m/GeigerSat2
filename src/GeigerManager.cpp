/**
  GeigerManager.cpp
  4/28/2023
  © Camren Mumme 2023
**/

// Includes
#include <RadiationWatch.h>
#include "LoggingManager.h"
#include "GeigerManager.h"

namespace GeigerManager {
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