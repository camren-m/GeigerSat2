/**
  LoggingManager.h
  4/28/2023
  © Camren Mumme 2023
**/
#pragma once

// Includes
#include <SD.h>

#define MAX_CHUNK_SIZE 64

namespace LoggingManager {
    File CurrentFile;

    class FileBufferMaster {
        private:
            arduino::String Buffer;
            arduino::String TargetFileName;
        public:
            FileBufferMaster(arduino::String FileName) {
                this->Buffer.reserve(1024);
                this->TargetFileName = FileName;
                SD.remove(this->TargetFileName);
            }
            void Add(arduino::String Message) {
                this->Buffer += Message;
            }
            void Update() {
                if(CurrentFile) {
                    CurrentFile.close();
                }
                CurrentFile = SD.open(this->TargetFileName, FILE_WRITE);
    
                int ChunkSize = CurrentFile.availableForWrite();
                if(ChunkSize > MAX_CHUNK_SIZE) {
                    ChunkSize = MAX_CHUNK_SIZE;
                }
                int BufferSize = this->Buffer.length();
                
                if(BufferSize >= ChunkSize && BufferSize > 0) {
                    digitalWrite(LED_BUILTIN, HIGH);
                    if(!CurrentFile.write(this->Buffer.c_str(), ChunkSize)) {
                        Serial.println("failed to write buffer. sd card may have been dislodged, removed, or corrupted");
                    }
                    digitalWrite(LED_BUILTIN, LOW);
                    this->Buffer.remove(0, ChunkSize);
                }
                CurrentFile.close();
            }
    };

    FileBufferMaster SystemLog("SYS.LOG");

    void Log(arduino::String Message) {
        arduino::String FormattedMessage = "["+arduino::String(millis())+"] "+Message+"\n";
        SystemLog.Add(FormattedMessage);
        Serial.print(FormattedMessage);
    }

    void Start() {
        pinMode(LED_BUILTIN, OUTPUT); // for FileBufferMaster write indications
        if(!SD.begin(10)) {
            Serial.println("Failed to initialize SD card. Please ensure you actually have one inserted you dumb*ss.");
            while(1) {};
        }
        Log("Initialized SD card.");
    }

    void Update() {
        SystemLog.Update();
    }
}