#include <Arduino.h>
#include <Wire.h>
#include "aht10.h"

#define I2C_SDA 8
#define I2C_SCL 9
#define AHT10_ADDR 0x38
#define AHT10_INIT 0xE1
#define AHT10_SOFT_RESET 0xBA
#define AHT10_TRIGGER_MEASUREMENT 0XAC
void setup() {
// write your initialization code here
   Serial.begin(115200);
   Serial.printf("Init complete");
   aht10_init();
}

void loop() {
// write your code here
   Serial.printf("Getting data...");
   Wire.beginTransmission(AHT10_ADDR);
   Wire.write(AHT10_TRIGGER_MEASUREMENT);
   Wire.write(0x33);
   Wire.write(0x00);
   Wire.endTransmission();
   delay(100);
   Wire.requestFrom(AHT10_ADDR,6);
   if (Wire.available() == 6) {
      uint8_t data[6];
      for (int i = 0; i < 6; i++) {
         data[i] = Wire.read();
      }

      uint32_t rawHumidity = data[1];
      rawHumidity<<=8;
      rawHumidity |= data[2];
      rawHumidity<<=4;
      rawHumidity |= data[3]>>4;
      //obliczenie wilgotnosci (dane/2^20)*100% [2^20 = 1048576]
      const double humidity = (rawHumidity * 100.0) / 1048576.0;

      // Print the results
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.printf("\n");
   }

   delay(1000);  // Wait before next reading
}

