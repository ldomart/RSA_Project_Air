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

void setup()
{
  Serial.begin(9600);
  Wire.begin(2);
  Wire.onRequest(requestEvent);

  data.a = 1;
  data.b = 2;
  data.c = 3;
  data.d = 4;
  data.e = 5.1;
  data.f = 6.1;
  data.g = 7.1;
  data.h = 8.1;
}

void loop()
{
  delay(100);
}

void requestEvent()
{
  Serial.print("sending ("); Serial.print(sizeof data); Serial.println(" bytes)");
  Wire.write((byte *)&data, sizeof data);
}