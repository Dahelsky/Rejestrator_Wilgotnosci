#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "aht10.h"

#include "secret.h"  //plik z parametrami sieci wifi, zawiera:
                     // SSID (nazwa sieci),
                     // PASS (hasło),s
                     // MQTTSERVER (adres brokera MQTT)
#define MQTT_TOPIC "esp32/hum"
#define SENSOR_INTERVAL 30000 //co ile (w ms) ma wykonywać pomiar

//#define DEBUG

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
#ifdef DEBUG
         Serial.print("\nWiFi error: ");
         Serial.println(WiFi.status());
#endif
         wifiCheckLastTime = now;
         WiFi.begin(SSID, PASS);
         delay(100);

      }
   }

   // sprawdzenie łączności MQTT
   if(mqttClient.state() != MQTT_CONNECTED) {
      if(now - mqttCheckLastTime > 5000) {
#ifdef DEBUG
         Serial.print("\nMQTT error: ");
         Serial.println((int)mqttClient.state());
#endif
         mqttCheckLastTime = now;
         mqttClient.connect("esp32-hum-sensor");
         delay(100);
      }
   }

   // odczyt z sensora
   uint16_t sensorInterval;
   SENSOR_INTERVAL > 4000 ? sensorInterval = SENSOR_INTERVAL - 1000 : sensorInterval = 4000;
   if(now - sensorCheckLastTime >(sensorInterval)) {
#ifdef DEBUG
      Serial.printf("\nOdczyt danych...\n");
#endif
      //wysłanie sygnału odczytu do aht10
      sensorCheckLastTime = now;
      if(aht10_read_on_command(aht10Data)) {
         //Pobranie zmierzona i obliczona wiglotnosc
         char buffer[10];
         snprintf(buffer,sizeof(buffer), "%.2f", aht10Data.humidity);
         mqttClient.publish(MQTT_TOPIC, buffer);
#ifndef DEBUG
      }
#else
         Serial.print("Wilgotnosc: ");
         Serial.print(buffer);
      }
      else
         Serial.printf("Blad odczytu");
#endif
   }
   mqttClient.loop();
}

