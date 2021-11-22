#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "setting.h"
#include "main.h"
#include "DHT.h"


//double _humid, double _tempr, int _noise, int ilum, bool _shock, bool _slide

#define PIN_DHT 3
#define PIN_VIB 2
#define PIN_CDS A0
#define PIN_MIC A1
#define PIN_cRTC 5
#define PIN_dRTC 4
#define PIN_rRTC 7
#define PIN_BUTN1 
#define PIN_BUTN2 
#define countof(a) (sizeof(a) / sizeof(a[0]))



double _humid;
double _tempr;
int _noise;
double _batt=0;
int _ilum;
bool _shock;
bool _slide;




DHT dht(PIN_DHT, DHT22);
ThreeWire myWire(PIN_dRTC, PIN_cRTC, PIN_rRTC);
RtcDS1302<ThreeWire> Rtc(myWire);


Tamagotchi game_tamagotchi = Tamagotchi();



void setup()
{
    game_tamagotchi.start_game();
    Serial.begin(9600);
    dht.begin();

    pinMode(PIN_VIB, INPUT);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

}

void loop()
{
    
    RtcDateTime now = Rtc.GetDateTime();
	_ilum = analogRead(PIN_CDS);
    _humid = dht.readHumidity();
    _tempr = dht.readTemperature();
    _shock = !(digitalRead(PIN_VIB) == HIGH);
    _noise = analogRead(PIN_MIC);
    
    printDateTime(now);

    if (!now.IsValid())
    {
        Serial.println("RTC lost confidence in the DateTime!");
    }

    game_tamagotchi.set_val_sensor(_humid, _tempr, _noise, _batt,  _ilum, _shock, _slide);
    game_tamagotchi.play();
    
    delay(500);
}

void Serial_print(double _humid, double _tempr, int _noise, int _lium, bool _shock){
    Serial.print(_humid);
    Serial.print("\t");
    Serial.print(_tempr);
    Serial.print("\t");
    Serial.print(_noise);
    Serial.print("\t");
    Serial.print(_ilum);
    Serial.print("\t");
    Serial.print(_shock);
    Serial.println("\n");

}

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
    Serial.print("\t");
}