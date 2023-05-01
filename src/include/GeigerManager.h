/**
  GeigerManager.h
  4/28/2023
  © Camren Mumme 2023
**/
#pragma once

// Includes
#include "RadiationWatch.h"

namespace GeigerManager {
    RadiationWatch ControlGeiger;
    RadiationWatch BariumGeiger;
    RadiationWatch BismuthGeiger;

    void Start();

    void Update();
}