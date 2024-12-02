#include <Wire.h>
#include <aht10.h>


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

bool aht10_read_on_command(aht10Data& aht10Data) {
    //wysłanie sygnału odczytu do aht10
    Wire.beginTransmission(AHT10_ADDR);
    Wire.write(AHT10_TRIGGER_MEASUREMENT);
    Wire.write(0x33);
    Wire.write(0x00);
    Wire.endTransmission();
    delay(100); // wymagane opoznienie od przesłania sygnału

    //odebranie 6 bajtów danych od sensora (temperatura i wilgotnosc)
    //1 bajt - informacje o stanie
    //2 bajt - wilgotnosc
    //3 bajt - wilgotnosc
    //4 bajt - 7-4 bit wilgotnosc, 3-0 bit temperatura
    //5 bajt - temperatura
    //6 bajt - temperatura

    Wire.requestFrom(AHT10_ADDR,6);
    if (Wire.available() == 6) {
        uint8_t data[6];
        //Serial.printf("");
        for (unsigned char & i : data) {
            i = Wire.read();
            /*Serial.print(i);
            Serial.print(" ");*/
        }
        //Serial.printf("done \n");
        aht10Data.state = data[0];

        uint32_t rawHumidity = data[1];
        rawHumidity<<=8;
        rawHumidity |= data[2];
        rawHumidity<<=4;
        rawHumidity |= data[3]>>4;
        aht10Data.humidity = (rawHumidity * 100.0) / 1048576.0; //wzór z datasheetu aht10

        //temperatura w celsjuszach
        uint32_t rawTemperature = data[3] & 0x0f;
        rawTemperature <<= 8;
        rawTemperature |= data[4];
        rawTemperature <<= 8;
        rawTemperature |= data[5];
        aht10Data.temperature = ((200*rawTemperature)/1048576.0) - 50;  //wzór z datasheetu aht10

        //opoźnienie zalecane przez producenta
        delay(1000);
        return true;
    }
    delay(1000);
    return false;

}
