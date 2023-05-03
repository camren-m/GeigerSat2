/**
  LoggingManager.cpp
  4/28/2023
  © Camren Mumme 2023
**/

// Preprocessor Directives
#define MAX_CHUNK_SIZE 64

// Includes
#include <SD.h>
#include "LoggingManager.h"

namespace LoggingManager {
    FileBufferMaster::FileBufferMaster(arduino::String FileName) {
        //this->Buffer.reserve(256);
        this->TargetFileName = FileName;
        SD.remove(this->TargetFileName.c_str());
    }
    void FileBufferMaster::Add(arduino::String Message) {
        this->Buffer += Message;
    }
    void FileBufferMaster::Update() {
        if (CurrentFile) {
            CurrentFile.close();
        }
        CurrentFile = SD.open(this->TargetFileName.c_str(), FILE_WRITE);

        int ChunkSize = CurrentFile.availableForWrite();
        if (ChunkSize > MAX_CHUNK_SIZE) {
            ChunkSize = MAX_CHUNK_SIZE;
        }
        int BufferSize = this->Buffer.length();

        if (BufferSize >= ChunkSize && BufferSize > 0) {
            digitalWrite(LED_BUILTIN, HIGH);
            if (!CurrentFile.write(this->Buffer.c_str(), ChunkSize)) {
                Serial.println(F("Possibly FATAL: Failed to write buffer. SD card may have been dislodged, removed, or corrupted. We will not wipe the buffer, but this may cause memory issues if this error reoccurs."));
                digitalWrite(LED_BUILTIN, LOW);
                return;
            }
            digitalWrite(LED_BUILTIN, LOW);
            this->Buffer.remove(0, ChunkSize);
        }
        CurrentFile.close();
    }

    void Log(arduino::String Message) {
        arduino::String FormattedMessage = "[" + arduino::String(millis()) + "] " + Message + "\n";
        Serial.print(FormattedMessage);
        SystemLog.Add(FormattedMessage);
    }

    void Start() {
        pinMode(LED_BUILTIN, OUTPUT); // for FileBufferMaster write indications
        if (!SD.begin(10)) {
            Serial.println(F("Failed to initialize SD card. Please ensure you actually have one inserted you dumb*ss."));
            while (1) { };
        }
        Log("Initialized SD card.");
    }

    void Update() {
        SystemLog.Update();
    }
}