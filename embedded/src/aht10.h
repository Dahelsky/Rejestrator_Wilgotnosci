#ifndef AHT10_H
#define AHT10_H
#include <arduino.h>


#define I2C_SDA 8
#define I2C_SCL 9
#define AHT10_ADDR 0x38
#define AHT10_INIT 0xE1
#define AHT10_SOFT_RESET 0xBA
#define AHT10_TRIGGER_MEASUREMENT 0XAC

struct aht10Data
{
    byte state;
    float humidity;
    float temperature;
};
void aht10_init();
bool aht10_read_on_command(aht10Data& aht10Data);


#endif //AHT10_H
