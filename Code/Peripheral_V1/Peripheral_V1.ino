/* Seeconda
*/
// Importing libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_PICO 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_PICO, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Global variable definition
byte Rxbyte;
bool output;

// Declaration for outside pins
#define TRIG 2
#define ECHO 3


void setup() {

  Wire.begin(0x09);        // join i2c bus (address is required for secondary)

  // Declaring the pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Setting up the display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay(); // clears the display
  display.display(); // Makes the changes appear
  display.setTextSize(1); // Formatting the text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Booting up..."); //Prints the message
  display.display();
  delay(1000); // Waits 1000 ms
  display.clearDisplay();
  display.display(); // Screen goes black again
}

void loop() {

  // DISPLAY SAMPLE DATA
  display.clearDisplay();
  display.setTextSize(2); 
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(changethishere); //Prints the distance
  display.display();

  delay(100); // Wait 100 milliseconds


 // RECEIVE A BYTE
  // Receive the potentiometer reading
  Wire.onReceive(receiveEvent);

  // // Send a byte
  // Wire.onRequest(requestEvent);
  
  // The function receives a byte and prints it on the serial monitor
}

void receiveEvent(){
  while(Wire.available()){
    Rxbyte = Wire.read();
    Serial.print("Byte: ");
    Serial.println(Rxbyte);
  }
}

// The function outputs a byte
void requestEvent(){
  Wire.write(output);
}

