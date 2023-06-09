/**
  main.cpp
  4/11/2023
  © Camren Mumme 2023
**/

// Preprocessor Macros
#include "RadiationWatch.h"
#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP3XX.h"
#include "pitches.h"
#define SEALEVELPRESSURE_HPA (1013.25) // CHANGE TO NEAREST WEATHER OBSERVATION STATION (E.G AIRPORT AWOS) SEA LEVEL PRESSURE BEFORE LAUNCH TO ENSURE PROPER ALTITUDE REPORTING

// Static Variables
RadiationWatch ControlGeiger(4,5);
RadiationWatch BariumGeiger(6,7);
RadiationWatch BismuthGeiger(8,9);
Adafruit_BMP3XX Altimeter;

int AliveNotes[] = {
  NOTE_C5,
};
int AliveDurations[] = {
  60,
};

int StartupNotes[] = {
  NOTE_A4, 0,  NOTE_A4, 0,  NOTE_A4, 0,  NOTE_F4, 0, NOTE_C5, 0,  NOTE_A4, 0,  NOTE_F4,
};
int StartupDurations[] = {
  50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 
};

int ErrorNotes[] = {
  NOTE_A4, 0,  NOTE_F4,
};
int ErrorDurations[] = {
  60, 10, 40, 
};

// Dynamic Variables
File CurrentFile;
unsigned long InitTime = 0;

// Functions
void PlayStartupTone() {
  for (int thisNote = 0; thisNote < 13; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / StartupDurations[thisNote];
    tone(3, StartupNotes[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(3);
  }
}

void PlayErrorTone() {
  for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / ErrorDurations[thisNote];
    tone(3, ErrorNotes[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(3);
  }
}

void PlayAliveTone() {
  for (int thisNote = 0; thisNote < 13; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / AliveDurations[thisNote];
    tone(3, AliveNotes[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(3);
  }
}


void Log(String Message) {
  CurrentFile.close();
  CurrentFile = SD.open("system.log", FILE_WRITE); 
  if(!CurrentFile) {
    Serial.print(F("["));
    Serial.print(millis()-InitTime);
    Serial.println(F("]        WARN: Failed to open logfile."));
    PlayErrorTone();
  }
  CurrentFile.print(F("["));
  CurrentFile.print(millis()-InitTime);
  CurrentFile.print(F("]        "));
  CurrentFile.println(Message);
  CurrentFile.close();

  Serial.print(F("["));
  Serial.print(millis()-InitTime);
  Serial.print(F("]       "));
  Serial.println(Message);
}

void WipeFile(String TargetFile) {  
  CurrentFile.close();
  if(!SD.exists(TargetFile)) return;
  SD.remove(TargetFile);
}

void CreateCSVHeader(String TargetFile) {
  CurrentFile.close();
  CurrentFile = SD.open(TargetFile, FILE_WRITE);

  CurrentFile.println(F("Time,Altitude,Count,CPM,uSvh,Error"));

  CurrentFile.close();
}

void LogReading(RadiationWatch Geiger, String CSVFile) {
  Log(CSVFile + " reading: " + String(Geiger.radiationCount()) + " counts " + String(Geiger.cpm()) + " CPM " + String(Geiger.uSvh()) + " uSv/h +/- " + String(Geiger.uSvhError()));
  
  CurrentFile.close();
  CurrentFile = SD.open(CSVFile, FILE_WRITE);
  CurrentFile.print(millis()-InitTime);
  CurrentFile.print(F(","));
  CurrentFile.print(Altimeter.readAltitude(SEALEVELPRESSURE_HPA));
  CurrentFile.print(F(","));
  CurrentFile.print(Geiger.radiationCount());
  CurrentFile.print(F(","));
  CurrentFile.print(Geiger.cpm());
  CurrentFile.print(F(","));
  CurrentFile.print(Geiger.uSvh());
  CurrentFile.print(F(","));
  CurrentFile.println(Geiger.uSvhError());
  CurrentFile.close();
}

void ControlGeigerRadiation() {
  LogReading(ControlGeiger, "control.csv");
}

void BariumGeigerRadiation() {
  LogReading(BariumGeiger, "barium.csv");
}

void BismuthGeigerRadiation() {
  LogReading(BismuthGeiger, "bismuth.csv");
}

void SetupGeiger(RadiationWatch *Geiger, String LogName, void (*RadiationCallback)(void), String Name) {
  WipeFile(LogName);
  CreateCSVHeader(LogName);
  Geiger->setup();
  Geiger->registerRadiationCallback(RadiationCallback);
  Log(Name+" setup");
}

void setup() {
  Serial.begin(9600);
  while(!Serial) { };
  Serial.println(F("Hello! Please send any serial message to begin initialization."));
  Serial.println(F("ANY DATA ON THE SD CARD WILL BE WIPED"));
  Serial.println(F("-------------------------------------------------------------"));
  PlayAliveTone();

  while(Serial.available() == 0) { };
  Serial.println(F("Proceeding..."));

  if (!SD.begin(10)) {
    Serial.println(F("SD initialization failed. Aborting."));
    PlayErrorTone();
    while (1);
  }

  InitTime = millis();

  CurrentFile.close();
  File InitLog = SD.open("system.log", FILE_WRITE);
  if(!InitLog) {
    Log("FATAL: Failed to open logfile on init. Aborting.");
    PlayErrorTone();
    while(1) { }
  }
  InitLog.close();
  

  WipeFile("system.log");
  Log(F("SD initialization successful."));

  if (!Altimeter.begin_I2C()) {
    Log(F("Altimeter I2C initialization failed. Aborting."));
    PlayErrorTone();
    while (1);
  }
  Altimeter.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  Altimeter.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  Altimeter.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  Altimeter.setOutputDataRate(BMP3_ODR_50_HZ);
  Log(F("Altimeter I2C initialization successful."));

  SetupGeiger(&ControlGeiger, "control.csv", &ControlGeigerRadiation, "Control Geiger");
  SetupGeiger(&BariumGeiger, "barium.csv", &BariumGeigerRadiation, "Barium Geiger");
  SetupGeiger(&BismuthGeiger, "bismuth.csv", &BismuthGeigerRadiation, "Bismuth Geiger");

  Log(F("Welcome! GeigerSat2 initialized."));
  Log(F("Copyright (c) 2023"));
  Log(F("   Camren Mumme. All Rights Reserved"));
  PlayStartupTone();
}

void loop() {
  if (!Altimeter.performReading()) {
    Log("Failed to perform alimeter reading. Altimeter data may not be accurate.");
    PlayErrorTone();
  }
  ControlGeiger.loop();
  BariumGeiger.loop();
  BismuthGeiger.loop();
}