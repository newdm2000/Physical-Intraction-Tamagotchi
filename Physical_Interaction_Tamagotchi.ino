#include <U8glib.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "setting.h"
#include "main.h"
#include "DHT.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
//double _humid, double _tempr, int _noise, int ilum, bool _shock, bool _slide

#define PIN_DHT 3
#define PIN_VIB 2
#define PIN_CDS A0
#define PIN_MIC A1
#define PIN_cRTC 5
#define PIN_dRTC 4
#define PIN_rRTC 7
#define PIN_BUTN1 8
#define PIN_BUTN2 9
#define PIN_BUTN3 10
#define PIN_BATT A2
#define countof(a) (sizeof(a) / sizeof(a[0]))



double _humid;
double _tempr;
int _noise;
double _batt=0;
double _ilum;
bool _shock;
bool _slide;
bool _meal;
bool _gm_butt;
int game_time;



DHT dht(PIN_DHT, DHT22);
ThreeWire myWire(PIN_dRTC, PIN_cRTC, PIN_rRTC);
RtcDS1302<ThreeWire> Rtc(myWire);
Tamagotchi game_tamagotchi;
RtcDateTime game_start_time;

void setup()
{
    Serial.begin(9600); //시리얼 정보
    dht.begin(); //dht22시작
    pinMode(PIN_VIB, INPUT);
    pinMode(PIN_BUTN1, INPUT);
    pinMode(PIN_BUTN2, INPUT);

    //Serial.print("compiled: ");
    //Serial.print(__DATE__);
    //Serial.println(__TIME__);

    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    //printDateTime(compiled);
    //Serial.println();

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
}

void loop()
{   
    RtcDateTime now = Rtc.GetDateTime();
	_ilum = analogRead(PIN_CDS);
    _humid = dht.readHumidity();
    _tempr = dht.readTemperature();
    _shock = !(digitalRead(PIN_VIB) == HIGH);
    _noise = analogRead(PIN_MIC);
    _slide = (digitalRead(PIN_BUTN2) == HIGH);
    _gm_butt = (digitalRead(PIN_BUTN1) == HIGH);
    _batt = BattCheck();
    _meal = (digitalRead(PIN_BUTN3) == HIGH);

    game_time = ((now.Hour()-game_start_time.Hour())*60 + (now.Minute()-game_start_time.Minute()))*60 + (now.Second()-game_start_time.Second());

    if(game_tamagotchi.get_Game_stste() == false) {
        Serial.println("Press Game Start Button!!");
        while(digitalRead(PIN_BUTN1) == LOW) {
        }
        game_tamagotchi.start_game();
        game_tamagotchi.set_Game_state(true);
        game_start_time = Rtc.GetDateTime();
    }

    //printDateTime(game_start_time);
    //printDateTime(now);

    if (!now.IsValid())
    {
        Serial.print("RTC lost confidence in the DateTime!");
    }
    //Serial.println(game_time);
    game_tamagotchi.set_val_sensor(_humid, _tempr, _noise, _batt, _ilum, game_time, _shock, _slide, _meal);
    //Serial_print(_humid, _tempr, _noise, _batt, _ilum, _shock, _slide);
    game_tamagotchi.play();
    u8g.firstPage(); //OLED 페이지 시작
    do{
        u8g.setFont(u8g_font_unifont);
        for(int i=0; i<9; i++){
            u8g.setPrintPos(10*i, 10);
            u8g.print(game_tamagotchi.get_em()[i]);
        }
        u8g.setPrintPos(0, 20);
        u8g.print(game_tamagotchi.get_t_HP());
        u8g.setPrintPos(0, 30);
        u8g.print(game_time);
    }while(u8g.nextPage());
}




void Serial_print(double _humid, double _tempr, int _noise, double _batt, double _ilum, bool _shock, bool _slide){
    Serial.print(_humid);
    Serial.print("\t");
    Serial.print(_tempr);
    Serial.print("\t");
    Serial.print(_noise);
    Serial.print("\t");
    Serial.print(_batt);
    Serial.print("\t");
    Serial.print(_ilum);
    Serial.print("\t");
    Serial.print(_shock);
    Serial.println("\t");
    Serial.print(_slide);
    Serial.println("\t");
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
    Serial.print("\n");
}

int BattCheck() {
	int vl=analogRead(PIN_BATT);
	int bl=map(vl,BATT_MIN,BATT_MAX,0,100);
	if (vl<BATT_MIN) bl=0;
	if (vl>BATT_MAX) bl=100;
	return bl;
}