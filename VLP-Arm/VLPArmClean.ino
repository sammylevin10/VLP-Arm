#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>
#include <CapacitiveSensor.h>

// Created by Sammy Levin for New York University Historic Fractures Museum Studies Seed Grant in Spring 2023
// sammylevin.com

CapacitiveSensor cs_4_8 = CapacitiveSensor(4, 8); // 1M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
CapacitiveSensor cs_1_10 = CapacitiveSensor(1, 10); // 1M resistor between pins 1 & 10, pin 10 is sensor pin, add a wire and or foil

File soundbyte;

bool debugging = false;
int thresholdPeg = 1000;
int thresholdHand = 3000;

String narrationFile = "output.wav";
String chantFile = "chant.wav";

void setup()
{
  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example
  cs_1_10.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example
  if (debugging)
  {
    Serial.begin(9600);
    Serial.print("Initializing SD card...");
    if (!SD.begin())
    {
      Serial.println(" Failed!");
    }
    Serial.println(" Initialized.");
  }
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop()
{
  long sensor1 = cs_4_8.capacitiveSensor(50);
  long sensor2 = cs_1_10.capacitiveSensor(50);
  if (debugging)
  {
    Serial.println(sensor1);
    Serial.println(sensor2);
  }

  // Capacitive touch detection on peg
  if (sensor1 >= thresholdPeg)
  {
    digitalWrite(7, HIGH);
    if (debugging) Serial.println("Peg triggered");
    soundbyte = SD.open(narrationFile);
    if (soundbyte)
    {
      AudioZero.begin(2 * 44100);
      AudioZero.play(soundbyte);
      soundbyte.close();
      AudioZero.end();
      if (debugging) Serial.println("End peg soundbyte");
    }
    else
    {
      if (debugging) Serial.println("Error opening " + narrationFile);
    }
  }
  else
  {
    digitalWrite(7, LOW);
  }
  // Capacitive touch detection on hand
  if (sensor2 >= thresholdHand)
  {
    digitalWrite(9, HIGH);
    if (debugging) Serial.println("Hand triggered");
    soundbyte = SD.open(chantFile);
    if (soundbyte)
    {
      AudioZero.begin(2 * 44100);
      AudioZero.play(soundbyte);
      soundbyte.close();
      AudioZero.end();
      if (debugging) Serial.println("End hand soundbyte");
    }
    else
    {
      if (debugging) Serial.println("Error opening " + chantFile);
    }
  }
  else
  {
    digitalWrite(9, LOW);
  }
}
