#include <Arduino.h>
#include <Wire.h>
#include "aht10.h"

aht10Data aht10Data;

void setup() {
// write your initialization code here
   Serial.begin(115200);
   Serial.printf("Init complete");
   aht10_init();

}

void loop() {
// write your code here
   Serial.printf("Getting data...\n");
   //wysłanie sygnału odczytu do aht10
   if(aht10_read_on_command(aht10Data)) {
      Serial.print("Humidity: ");
      Serial.print(aht10Data.humidity);
      Serial.print("\t Temperature: ");
      Serial.print(aht10Data.temperature);
      Serial.print("\n");
   }
   else
      Serial.printf("Cannot read data");
}

