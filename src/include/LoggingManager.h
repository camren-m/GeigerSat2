/**
  LoggingManager.h
  4/28/2023
  © Camren Mumme 2023
**/
#pragma once

// Includes
#include <SD.h>

namespace LoggingManager {
    void Log(arduino::String Message);

    void Start();

    void Update();
}