#include <Wire.h>
#include <RTClib.h>
#include <dht.h>

#define DHT11_PIN 7
dht DHT;
RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  delay(2000);
  Wire.begin();
  delay(2000);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    delay(5000);;  // Halt if RTC not found
  }
}

void loop() {
  // Read DHT11
  int chk = DHT.read11(DHT11_PIN);
  if (chk == DHTLIB_OK) {
    Serial.print("Temperature: ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity: ");
    Serial.println(DHT.humidity);
    Serial.println("---");
  } else {
    Serial.print("DHT read error: ");
    Serial.println(chk);
  }

  // Read RTC
  DateTime now = rtc.now();
  int hour = now.hour();
  Serial.print("Hour: ");
  Serial.println(hour);
  Serial.println("---");

  int moisture = analogRead(A0);
  float moisturePercent = map(moisture, 0, 1023, 100, 0);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");


  int light = analogRead(A1);
  Serial.print("Light: ");
  Serial.println(light);
  delay(2000);
}
