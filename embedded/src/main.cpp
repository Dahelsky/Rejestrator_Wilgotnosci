#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "aht10.h"


// jesli istnieje plik secret.h to ma go includowac
// secret.h ma define'y z SSID, PASS i MQTTSERVER
// nie chce mi sie z każdym commitem chować danych sieci
#define SECRET

#ifdef SECRET
#include "secret.h" //plik z parametrami sieci wifi
#endif

// tu wstawić swoje parametry
#ifndef SSID
#define SSID "SSID"
#endif
#ifndef PASS
#define PASS "PASS"
#endif
#ifndef MQTTSERVER
#define MQTTSERVER "MQTTSERVER"
#endif

#define MQTT_TOPIC "esp32/hum"

aht10Data aht10Data;


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
   Serial.begin(115200);
   mqttClient.setServer(MQTTSERVER,1883);

   WiFi.mode(WIFI_STA);
   WiFi.begin(SSID, PASS);
   delay(2000);
   mqttClient.connect("esp32-hum-sensor");
   aht10_init();

}
unsigned long sensorCheckLastTime = 0;
unsigned long wifiCheckLastTime = 0;
unsigned long mqttCheckLastTime = 0;

void loop() {
   unsigned long now = millis();

   // sprawdzenie łączności WiFi
   if(WiFi.status() != WL_CONNECTED) {
      if(now - wifiCheckLastTime > 5000) {
         Serial.print("\nWiFi error: ");
         Serial.println(WiFi.status());
         wifiCheckLastTime = now;
         WiFi.begin(SSID, PASS);
         delay(100);

      }
   }

   // sprawdzenie łączności MQTT
   if(mqttClient.state() != MQTT_CONNECTED) {
      if(now - mqttCheckLastTime > 5000) {
         Serial.print("\nMQTT error: ");
         Serial.println((int)mqttClient.state());
         mqttCheckLastTime = now;
         mqttClient.connect("esp32-hum-sensor");
         delay(100);
      }
   }

   // odczyt z sensora
   if(now - sensorCheckLastTime >4000) {
      //Serial.print(now - sensorCheckLastTime);
      Serial.printf("\nOdczyt danych...\n");
      //wysłanie sygnału odczytu do aht10
      sensorCheckLastTime = now;
      if(aht10_read_on_command(aht10Data)) {
         //Pobranie zmierzona i obliczona wiglotnosc
         Serial.print("Wilgotnosc: ");
         char buffer[10];
         snprintf(buffer,sizeof(buffer), "%.2f", aht10Data.humidity);
         Serial.print(buffer);
         mqttClient.publish(MQTT_TOPIC, buffer);
      }
      else
         Serial.printf("Blad odczytu");
   }
   mqttClient.loop();
}
