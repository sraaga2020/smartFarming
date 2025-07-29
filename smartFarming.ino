#include <Wire.h>
#include <RTClib.h>
#include <dht.h>

#define DHT11_PIN 7
dht DHT;

RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(2000);
  if (!rtc.begin()) {
    // If RTC not found, halt
    while (1);
  }
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  if (chk == DHTLIB_OK) {
    Serial.print("Temp: ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity: ");
    Serial.println(DHT.humidity);
  } else {
    Serial.print("Read error: ");
    Serial.println(chk);
  }

  int moisture = analogRead(A0);
  Serial.print("Moisture: ");
  Serial.println(moisture);

  int light = analogRead(A1);
  Serial.print("Light: ");
  Serial.println(light);

  DateTime now = rtc.now();
  Serial.print("Hour: ");
  Serial.println(now.hour());
 

  delay(1000);
}
