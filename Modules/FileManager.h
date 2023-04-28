/**
  FileManager.h
  4/28/2023
  © Camren Mumme 2023
**/
#pragma once

// Includes
#include "SD.h"

namespace FileManager {
    File CurrentFile;

    void Start() {
      if(!SD.begin(10)) {

      }
    }
}