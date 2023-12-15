/* Robotic Sensors and Actuators
  This device monitors the carbon dioxyde, humidity, and temperature levels.
  It runs on an Arduino UNO R3 with a DHT11 humidity sensor, a GY-SGP gas sensor, a piezoelectric buzzer and an Oled Display
  Please refer to the project report for further setup instructions.
  12/15/2023
  Catherine Pollard, David Hindman, Alexander Pivovarov, Louis Domart
*/

#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <dht11.h>
#define DHT11PIN 5 // Humidity sensor pin
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#define potPin A0 // Potentiometer pin
#define buzPin 6 // Buzzer pin
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define ctof_buttonpin 7 // Button pin for converting c to f
// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_PICO 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_PICO, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
dht11 DHT11;
Adafruit_SGP30 sgp;

// Current usage: 70 mA at 5V

/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature));  // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                 // [mg/m^3]
  return absoluteHumidityScaled;
}

// Threshold variables for potentiometer
float max_ppm = 1500.0; // Maximum CO2 treshold the user can set
float min_ppm = 200.0; // Minimum CO2 value

// Definition of global variables
int counter = 0;
int static potVoltage;
float conversion_factor = (1500.0-min_ppm)/1023.0;
int temperaturef = 0;
float temperaturec = 0;
int buttoncount = 1;

void setup() {
  
  Serial.begin(115200)
  while (!Serial) { delay(10); }  // Wait for serial console to open!

  Serial.println("SGP30 test");
  // Initiate the gas sensor
  if (!sgp.begin()) {
    Serial.println("Sensor not found :(");
    while (1)
      ;
  }
  
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // Setting up the display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();  // clears the display
  display.display();       // Makes the changes appear
  display.setTextSize(1);  // Formatting the text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Booting up...");  //Prints the message
  display.display();
  delay(1000);  // Waits 1000 ms
  display.clearDisplay();
  display.display();  // Screen goes displays what was printed
  display.println("Remember that min CO2 safety is 200 ppm");
  display.display();
  delay(5000);
  display.clearDisplay();

  //potentiometer and buzzer pins setup
  pinMode(potPin, INPUT);
  pinMode(buzPin, OUTPUT);
  pinMode(ctof_buttonpin, INPUT_PULLUP);
}

void loop() {
  // Read the pushbutton value
  if (digitalRead(ctof_buttonpin) == LOW){
  buttoncount++;
  }
  
  display.clearDisplay();
  display.setCursor(0, 0);
  potVoltage = analogRead(potPin);
  max_ppm = min_ppm + potVoltage * conversion_factor;
  display.print("Max. CO2 is: ");
  display.print((int)max_ppm);
  display.println(" ppm");
  Serial.print(max_ppm);

  // Reading the humidity and temperature
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);

  // Setting the absolute humidity on the gas sensor to enable the humditiy compensation for the air quality signals
  float temperature = DHT11.temperature; // [°C]
  float humidity = DHT11.humidity; // [%RH]
  temperaturef = 32 + temperature*1.8;
  temperaturef = int(temperaturef);
  temperaturec = temperature;
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
  
  // Error proofing
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  // Printing the gas readings on the serial monitor
  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  //displays temperature, eCO2 content and humidity
  display.print("eCO2 "); display.print(sgp.eCO2); display.println(" ppm");
  if (buttoncount % 2 == 1) {
    display.print("Temperature "); display.print(temperaturec); display.println(" C");
  }
  else{
    display.print("Temperature "); display.print(temperaturef); display.println(" F");
  }
  display.print("Humidity "); display.print(humidity); display.println(" %");
  
  // Ringing the alarm for low CO2 threshold
  if (sgp.eCO2 < 200.0){
    for (int i=0; i<3; i++){
      tone(buzPin, 1000);
      delay(500);
      noTone(buzPin);
      delay(500);
    }
  }
  else{
    noTone(buzPin);
  }
  // Ringing the alarm for max CO2 threshold
  if (sgp.eCO2 > max_ppm){
    for (int i=0; i<3; i++){
      tone(buzPin, 1000);
      delay(500);
      noTone(buzPin);
      delay(500);
    }
  }
  else{
    noTone(buzPin);
  }

  // Possibility of measuring H2 and ethanol concentration in the air as well
  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");
  display.display();
  delay(2000);
}