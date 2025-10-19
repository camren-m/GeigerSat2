# GeigerSat2
Hardware plans, manufacturing details, source code, and FreeCAD documents relating to the GeigerSat2 project, launched aboard 
[Perlan 2's 2023 flight campaign in Argentina](https://perlanproject.org/2023/).

![Picture of the finished CubeSat](https://github.com/camren-m/GeigerSat2/assets/61283706/24af0c9d-7978-410f-9d94-899d2b5a57df)

## Tech stack
At the heart of the CubeSat, we have an Arduino Nano Every running on the Arduino framework, with the firmware code found in ``src/main.cpp``. (the version found on this repo is not the final flight firmware, pending update)

The Arduino Nano Every interacts with an Adafruit ``BMP388``, an Adafruit microSD card module, buzzer, and up to 3 RadiationWatch Type 5
solid state geiger counters. All components (excluding the geiger counters) are soldered to the custom PCB for simplicity, reliability,
and smaller size. The geiger counters are connected through JST-XH connectors on the PCB.

![Larger components soldered](https://github.com/camren-m/GeigerSat2/assets/61283706/3a9b0110-adea-47a0-a2e4-74acace67f0e)
![Smaller components soldered](https://github.com/camren-m/GeigerSat2/assets/61283706/5dcbd664-6612-4bcf-a86a-f2ef8a0abe96)
![The small, solid-state geiger counter used](https://github.com/camren-m/GeigerSat2/assets/61283706/e3650d8f-3da2-4996-99f7-fd9fc161261c)

Power is provided through a 2500mah 3.7V lithium-polymer battery, boosted to 5V through an Adafruit PowerBoost board, and fed into the
Arduino Nano Every's USB port. An option of supplying 7-12V DC to the 2 pin JST-XH header is an alternative. It will be passed directly
to the Arduino Nano Every's VCC pin, and subsqeuently through its on-board voltage regulator.

## The experiment
More to be added soon.
![The compounds used to create the two radiation shielding material candidates](https://github.com/camren-m/GeigerSat2/assets/61283706/9b192b44-0e74-4a17-b319-3f0148a3206c)

