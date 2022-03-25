#define BLYNK_TEMPLATE_ID "TMPLE0P22IbY"
#define BLYNK_DEVICE_NAME "health monitoring"
#define BLYNK_AUTH_TOKEN "FdkITGwQUxfKBdRhytelGVAn_NIdO877"

#define BLYNK_PRINT Serial
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define REPORTING_PERIOD_MS 1000
#define DS18B20 2
#define VPIN_BUTTON_1    V3
#define VPIN_BUTTON_2    V4
#define VPIN_BUTTON_3    V5
#define VPIN_BUTTON_4    V6 
#define VPIN_BUTTON_5   V7
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Omnytrix labs";
char pass[] = "Omnytrix@123";
PulseOximeter pox;

uint8_t DHTPin = 14;
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
float bodytemperature;
float BPM, SpO2;
uint32_t tsLastReport = 0;

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);

void onBeatDetected()
{
  Serial.println("Beat Detected!");
}

BLYNK_CONNECTED() {

  // Request the latest state from the server

  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
   Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(VPIN_BUTTON_5);
}

void setup()
{
  Serial.begin(115200);
  pinMode(16, OUTPUT);
 pinMode(DHTPin, INPUT);
 dht.begin();
  Blynk.begin(auth, ssid, pass);
  Serial.print("Initializing Pulse Oximeter..");

  if (!pox.begin())
  {
    Serial.println("FAILED");
    for (;;);
  }
  else
  {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop()
{
  pox.update();
  Blynk.run();
 Temperature = dht.readTemperature(); // Gets the values of the temperature
 Humidity = dht.readHumidity(); // Gets the values of the humidity
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  //bodytemperature = sensors.getTempCByIndex(0);

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print(" bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println(" %");
    Serial.print("Room Temperature: ");
    Serial.print(Temperature);
    Serial.println("°C");
    Serial.print("Room Humidity: ");
    Serial.print(Humidity);
    Serial.println("%");
    Serial.print("Body Temperature: ");
    Serial.print(bodytemperature);
    Serial.println("°C");
    Blynk.virtualWrite(V3, Temperature);
    Blynk.virtualWrite(V4, Humidity);
    Blynk.virtualWrite(V5, BPM);
    Blynk.virtualWrite(V6, SpO2);
    Blynk.virtualWrite(V7, bodytemperature);

    tsLastReport = millis();
  }
}
