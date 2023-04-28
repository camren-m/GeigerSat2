/**
  GeigerSat2.ino
  4/11/2023
  © Camren Mumme 2023
**/
/*
// Preprocessor Macros
#include "RadiationWatch.h"
#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP3XX.h"
#define SEALEVELPRESSURE_HPA (1013.25) // CHANGE TO NEAREST WEATHER OBSERVATION STATION (E.G AIRPORT AWOS) SEA LEVEL PRESSURE BEFORE LAUNCH TO ENSURE PROPER ALTITUDE REPORTING

// Static Variables
RadiationWatch ControlGeiger(2,3);
RadiationWatch BariumGeiger(4,5);
RadiationWatch BismuthGeiger(6,7);
Adafruit_BMP3XX Altimeter;

// Dynamic Variables
File CurrentFile;
unsigned long InitTime = 0;

// Functions
void Log(String Message) {
  CurrentFile.close();
  CurrentFile = SD.open("system.log", FILE_WRITE); 
  if(!CurrentFile) {
    Serial.print(F("["));
    Serial.print(millis()-InitTime);
    Serial.println(F("]        WARN: Failed to open logfile."));
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

  while(Serial.available() == 0) { };
  Serial.println(F("Proceeding..."));

  if (!SD.begin(10)) {
    Serial.println(F("SD initialization failed. Aborting."));
    while (1);
  }

  InitTime = millis();

  CurrentFile.close();
  File InitLog = SD.open("system.log", FILE_WRITE);
  if(!InitLog) {
    Log("FATAL: Failed to open logfile on init. Aborting.");
    while(1) { }
  }
  InitLog.close();
  

  WipeFile("system.log");
  Log(F("SD initialization successful."));

  if (!Altimeter.begin_I2C()) {
    Log(F("Altimeter I2C initialization failed. Aborting."));
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

  Log("Welcome! GeigerSat2 initialized.");
  Log("Copyright (c) 2023");
  Log("   Camren Mumme. All Rights Reserved");
}

void loop() {
  if (!Altimeter.performReading()) {
    Log("Failed to perform alimeter reading. Altimeter data may not be accurate.");
  }
  ControlGeiger.loop();
  BariumGeiger.loop();
  BismuthGeiger.loop();
}
*/