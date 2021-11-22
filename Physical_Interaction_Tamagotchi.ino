#include "setting.h"
#include "main.h"

double humi;
double temp;
int ilum;
bool shock;

DHT dht(PIN_DHT, DHT22);
void Serial_print(double ilum, double humi, int temp, bool shock);

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
    Serial.print(ilum);
    Serial.print("\t");
    Serial.print(humi);
    Serial.print("\t");
    Serial.print(temp);
    Serial.print("\t");
    Serial.print(shock);
    Serial.println("\n");

    delay(1000);
    //serial_print(ilum, humi, temp, shock);
}

/*void Serial_print(double ilum, double humi, int temp, bool shock){
    
}*/

