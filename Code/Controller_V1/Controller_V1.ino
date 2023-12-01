/*
This is the controller program for an I2C arduino communication setup of the environment monitoring system.
11/28/2023
*/

// Including the library for I2C
#include <Wire.h>

// Pin wiring definition
#define potPin A0
#define thermPin A1
#define btn1Pin 8
#define btn2Pin 7
#define buzzerPin 6

// Global variable definition
bool readVar;

void setup() {
  Wire.begin();        // join i2c bus (address optional for main)
  //Serial.begin(9600);  // start serial for output

  // Define the pin modes
  pinMode(potPin, INPUT);
  pinMode(thermPin, INPUT);
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Define the variables
  int static potVoltage;
  int static thermVoltage;
  float static voltageLevel;
  byte static Txbyte;

  potVoltage = analogRead(potPin);
  // Note that analogRead gives a value between 0-1023
  
  
  // Sending the I2C byte
  Wire.beginTransmission(0x09); // Send to the corresponding address
  Wire.write(potVoltage);
  Wire.endTransmission();

  delay(100);
}
