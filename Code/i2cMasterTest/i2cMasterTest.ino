#include <Wire.h>

struct TransmitData
{
  int32_t a;
  int32_t b;
  int32_t c;
  int32_t d;
  float e;
  float f;
  float g;
  float h;
};

TransmitData data;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Serial.print("requesting ("); Serial.print(sizeof data); Serial.print(" bytes)... ");
  if (Wire.requestFrom(2, sizeof data)) {
    Wire.readBytes((byte*) &data, sizeof data);
    Serial.println("done");

    Serial.println(data.a);
    Serial.println(data.b);
    Serial.println(data.c);
    Serial.println(data.d);
    Serial.println(data.e);
    Serial.println(data.f);
    Serial.println(data.g);
    Serial.println(data.h);
  } 
  else {
    Serial.println("could not connect");
  }
  delay(500);
}