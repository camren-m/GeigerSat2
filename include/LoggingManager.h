/**
  LoggingManager.h
  4/28/2023
  © Camren Mumme 2023
**/

#pragma once

// Includes
#include "SD.h"

namespace LoggingManager {
    static File CurrentFile;

    class FileBufferMaster {
        private:
            arduino::String Buffer;
            arduino::String TargetFileName;
        public:
            FileBufferMaster(arduino::String FileName);
            void Add(arduino::String Message);
            void Update();
    };

    static FileBufferMaster SystemLog("SYS.LOG");

    void Log(arduino::String Message);

    void Start();

    void Update();
}
