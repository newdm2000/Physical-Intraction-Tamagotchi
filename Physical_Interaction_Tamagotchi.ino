#include "setting.h"
#include "main.h"
#include <DHT.h>

#define PIN_DHT 3
#define PIN_VIB 2
#define PIN_CDS A0

double humi;
double temp;
int ilum;
bool shock;

DHT dht(PIN_DHT, DHT22);

void setup()
{
    Serial.begin(9600);
    dht.begin();
    pinMode(PIN_VIB, INPUT);
}

void loop()
{
	ilum = analogRead(PIN_CDS);
    humi = dht.readHumidity();
    temp = dht.readTemperature();
    shock = (digitalRead(PIN_VIB) == HIGH);

    Serial_print(ilum, humi, temp, shock);
    delay(1000);
}

void Serial_print(double _ilum, double _humid, int _tempr, bool _shock){
    Serial.print(_ilum);
    Serial.print("\t");
    Serial.print(_humid);
    Serial.print("\t");
    Serial.print(_tempr);
    Serial.print("\t");
    Serial.print(_shock);
    Serial.println("\n");
}

