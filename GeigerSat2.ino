/**
  GeigerSat2.ino
  4/28/2023
  © Camren Mumme 2023
**/

// Includes
#include "Modules/FileManager.h"
#include "Modules/CommandManager.h"
#include "Modules/AltimeterManager.h"
#include "Modules/GeigerManager.h"

void setup() {
    Serial.begin(9600);
    while(!Serial) { }

    Serial.println("GeigerSat2 ready for initialization. Send proceed command to initialize.");
    Serial.println("------------------------------------------------------------------------");
    CommandManager::AwaitProceedCommand();
  
    FileManager::Start();
    AltimeterManager::Start();
    GeigerManager::Start();
}

void loop() {
    CommandManager::Update();
}