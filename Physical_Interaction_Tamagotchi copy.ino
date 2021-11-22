#include "setting.h"
#include "main.h"

double humi;
double temp;
int ilum;


DHT dht(PIN_DHT, DHT22);

void setup()
{
    Serial.begin(9600);
    dht.begin();
    pinMode(PIN_VIB, INPUT);
}

void loop()
{
	int sensor = analogRead(PIN_CDS);
    humi = dht.readHuminity();
    temp = dht.readTemperature();
    
}
