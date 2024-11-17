#include <Wire.h>

#define I2C_SDA 8
#define I2C_SCL 9
#define AHT10_ADDR 0x38
#define AHT10_INIT 0xE1
#define AHT10_SOFT_RESET 0xBA
#define AHT10_TRIGGER_MEASUREMENT 0XAC

void aht10_init()
{
    Wire.begin(I2C_SDA,I2C_SCL, 10000);
    //Poczatek ustawien sensora
    //pierwszy bajt - adres
    Wire.beginTransmission(AHT10_ADDR); //adres aht10
    //drugi bajt - komenda
    Wire.write(AHT10_INIT); //inicjalizacja
    //trzeci bajt - opcje stanu
    Wire.write(0x00|(1<<6)|(1<<3)); //tryb cmd (tu chodzi o bity [6:5]) + tryb kalibracji [3]
    Wire.write(0x00); //nop
    Wire.endTransmission();
    //delay(20); // min 20 ms opoznienia by sie to zresetowalo
    delay(100);
}

void aht10_read_cmd() {

}