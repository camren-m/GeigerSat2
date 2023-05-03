/**
  GeigerManager.h
  4/28/2023
  © Camren Mumme 2023
**/

#pragma once

// Includes
#include "RadiationWatch.h"

namespace GeigerManager {
    static RadiationWatch ControlGeiger(2,3);
    static RadiationWatch BariumGeiger(4,5);
    static RadiationWatch BismuthGeiger(6,7);

    void Start();

    void Update();
}
