#include <Wire.h>
#include <RTClib.h>
#include <dht.h>
#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>

#define DHT11_PIN 7
#define MODEL ST7796S
#define CS   10
#define CD   9
#define RST  8
#define MOSI 11
#define MISO 12
#define SCK  13
#define LED  2
#define WEB_BG_GREEN    0xF79E
#define WEB_FIELD_GREEN 0xD72E
#define WEB_TEXT_DARK   0x1A44
#define WHITE           0xFFFF
#define ORANGE          0xFD20

dht DHT;
RTC_DS1307 rtc;
const int pumpPin = 4;
LCDWIKI_SPI mylcd(MODEL,CS,CD,MISO,MOSI,RST,SCK,LED);

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mylcd.Fill_Screen(WEB_BG_GREEN);

  mylcd.Set_Draw_color(WHITE);
  mylcd.Fill_Round_Rectangle(20, 10, 460, 310, 10);

  int icon_x = 80;
  int icon_y = 40;

  mylcd.Set_Draw_color(WEB_FIELD_GREEN);
  mylcd.Fill_Circle(icon_x, icon_y, 15);

  mylcd.Set_Draw_color(WHITE);
  mylcd.Draw_Line(icon_x, icon_y - 7, icon_x, icon_y + 10);
  mylcd.Draw_Line(icon_x, icon_y, icon_x - 7, icon_y - 5);
  mylcd.Draw_Line(icon_x, icon_y, icon_x + 7, icon_y - 5);

  mylcd.Set_Text_colour(WEB_TEXT_DARK);
  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Your Smart Garden", icon_x + 30, 25);

  mylcd.Set_Draw_color(WEB_FIELD_GREEN);
  mylcd.Set_Text_Back_colour(WEB_FIELD_GREEN);
  mylcd.Set_Text_Size(2);

  mylcd.Fill_Round_Rectangle(40, 75, 240, 145, 8);
  mylcd.Print_String("Temperature:", 55, 85);
  mylcd.Fill_Round_Rectangle(260, 75, 440, 145, 8);
  mylcd.Print_String("Humidity:", 275, 85);
  mylcd.Fill_Round_Rectangle(40, 155, 240, 225, 8);
  mylcd.Print_String("Soil Moisture:", 55, 165);
  mylcd.Fill_Round_Rectangle(260, 155, 440, 225, 8);
  mylcd.Print_String("Light Level:", 275, 165);

  mylcd.Fill_Round_Rectangle(40, 235, 440, 295, 8);
  pinMode(pumpPin, OUTPUT);

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
  int temperature = 0;
  int humidity = 0;
  int chk = DHT.read11(DHT11_PIN);
  if (chk == DHTLIB_OK) {
    temperature = DHT.temperature;
    humidity = DHT.humidity;
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
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
  float moisturePercent = abs(moisture*100/1023);

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  int light = analogRead(A1);
  Serial.print("Light: ");
  Serial.println(light);
  delay(2000);

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "WATER") {
      digitalWrite(pumpPin, HIGH);
      delay(3000);
      digitalWrite(pumpPin, LOW);
    }
    if (command == "NOWATER") {
      digitalWrite(pumpPin, LOW);
    }
  }


  mylcd.Set_Text_colour(WEB_TEXT_DARK);
  mylcd.Set_Text_Back_colour(WEB_FIELD_GREEN);
  mylcd.Set_Text_Size(2);
  mylcd.Set_Draw_color(WEB_FIELD_GREEN);

  mylcd.Fill_Rectangle(55, 115, 225, 135);
  mylcd.Print_String(String(temperature, 1)+" C", 90, 115);

  mylcd.Fill_Rectangle(275, 115, 425, 135);
  mylcd.Print_String(String(humidity)+" %", 320, 115);

  mylcd.Fill_Rectangle(55, 195, 225, 215);
  mylcd.Print_String(String(moisturePercent)+" %", 100, 195);

  mylcd.Fill_Rectangle(275, 195, 425, 215);
  mylcd.Print_String(String(light), 320, 195);
}
