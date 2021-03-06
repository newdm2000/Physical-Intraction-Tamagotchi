#include <U8glib.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include <SD.h>
#include <SPI.h>
#include <string.h>
#include "setting.h"
#include "main.h"
#include "DHT.h"

using namespace std;

//define some colour values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
//double _humid, double _tempr, int _noise, int ilum, bool _shock, bool _slide
#define PIN_DHT 22
#define PIN_VIB 23
#define PIN_CDS A7
#define PIN_MIC A6
#define PIN_cRTC 50
#define PIN_dRTC 51
#define PIN_rRTC 52
#define PIN_BUTN1 24
#define PIN_BUTN2 25
#define PIN_BUTN3 26
#define PIN_BATT A10
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
LCDWIKI_KBV mylcd(ILI9486,A3,A2,A1,A0,A4);
RtcDS1302<ThreeWire> Rtc(myWire);
Tamagotchi game_tamagotchi;
RtcDateTime game_start_time;

void setup()
{
    Serial.begin(9600); //시리얼 정보
    mylcd.Init_LCD();
    mylcd.Fill_Screen(BLACK);
    mylcd.Set_Text_Back_colour(BLACK);
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Mode(0);
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
    mylcd.Set_Rotation(3);
    RtcDateTime now = Rtc.GetDateTime();
	_ilum = 1024 - analogRead(PIN_CDS);
    _humid = dht.readHumidity();
    _tempr = dht.readTemperature();
    Serial.println(_tempr);
    _shock = !(digitalRead(PIN_VIB) == HIGH);
    _noise = analogRead(PIN_MIC);
    _slide = (digitalRead(PIN_BUTN2) == LOW);
    _gm_butt = (digitalRead(PIN_BUTN1) == HIGH);
    _batt = BattCheck();
    _meal = (digitalRead(PIN_BUTN3) == LOW);


    if(game_tamagotchi.get_Game_stste() == false) {
        mylcd.Fill_Screen(BLACK);
        Serial.println("Press Game Start Button!!");
        mylcd.Set_Text_Size(3);
        mylcd.Print_String("Press Game Start Button!!", 0, 0);
        while(digitalRead(PIN_BUTN1) == HIGH) {
        }
        mylcd.Fill_Screen(BLACK);
        game_tamagotchi.start_game();
        game_tamagotchi.set_Game_state(true);
        game_start_time = Rtc.GetDateTime();
    }
    if (game_tamagotchi.get_game_step() > MAX_STEP) {
        mylcd.Fill_Screen(BLACK);
        mylcd.Set_Text_Size(5);
        mylcd.Print_String("Congratulations!!!!!!", 0, 0);
        mylcd.Set_Text_Size(2);
        mylcd.Print_String("Press Game Start Button!!", 0, 5);
        while(digitalRead(PIN_BUTN1) == HIGH) {
        }
        mylcd.Fill_Screen(BLACK);
        game_tamagotchi.start_game();
        game_tamagotchi.set_Game_state(false);
    }

    game_time = ((now.Hour()-game_start_time.Hour())*60 + (now.Minute()-game_start_time.Minute()))*60 + (now.Second()-game_start_time.Second());

    //printDateTime(game_start_time);
    //printDateTime(now);

    if (!now.IsValid())
    {
        Serial.print("RTC lost confidence in the DateTime!");
    }
    //Serial.println(game_time);
    if(game_time>0) {
        game_tamagotchi.set_val_sensor(_humid, _tempr, _noise, _batt, _ilum, game_time, _shock, _slide, _meal);
    //Serial_print(_humid, _tempr, _noise, _batt, _ilum, _shock, _slide);
        game_tamagotchi.play();

    
    //if(game_time % 10 == 0) mylcd.Fill_Screen(BLACK);
        mylcd.Set_Text_Size(3);
        if (print_emotion_str() != NULL) {
            mylcd.Print_String(print_emotion_str(), 0, 0);
        }
        else {
                mylcd.Set_Draw_color(BLACK);
        mylcd.Fill_Rectangle(0,0,mylcd.Get_Display_Width(),80);
        }
        mylcd.Print_String("HP: ", 0, 90);
        mylcd.Print_Number_Int(game_tamagotchi.get_t_HP(), 100, 90, 5, ' ', 10);
        mylcd.Print_String("Time: ", 0, 120);
        mylcd.Print_Number_Int(game_time, 100, 120, 5, ' ', 10);
        mylcd.Print_String("Step: ", 0, 150);
        mylcd.Print_Number_Int(game_tamagotchi.get_game_step(), 100, 150, 5, ' ', 10);

        mylcd.Set_Draw_color(YELLOW);
        mylcd.Set_Text_Size(2);
        mylcd.Print_String("Start Game", 50, 205);
        mylcd.Fill_Circle(200, 210, 20);
        mylcd.Print_String("Feeding", 315, 205);
        mylcd.Fill_Circle(280, 210, 20);
        mylcd.Print_String("Stroking", 110, 265);
        mylcd.Fill_Circle(240, 270, 20);
        
        mylcd.Set_Text_Size(1);
        mylcd.Print_String("Day time(s):", 350, 280);
        mylcd.Print_Number_Int(TIME_DAYTIME, 450, 280, 5, ' ', 10);
        mylcd.Print_String("Night time(s):", 350, 290);
        mylcd.Print_Number_Int(TIME_NIGHT, 450, 290, 5, ' ', 10);
        mylcd.Print_String("Feeding time(s):", 350, 300);
        mylcd.Print_Number_Int(TIME_MEAL, 450, 300, 5, ' ', 10);
        mylcd.Print_String("Final Step:", 350, 310);
        mylcd.Print_Number_Int(MAX_STEP, 450, 310, 5, ' ', 10);
    }
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

String print_emotion_str(){
    String str;
    if (game_tamagotchi.get_em()[0] == 0) {
        str += "wet ";
    }
    else if(game_tamagotchi.get_em()[0] == 2) {
        str += "dry ";
    }
    else {}

    if (game_tamagotchi.get_em()[1] == 0) {
        str += "hot ";
    }
    else if (game_tamagotchi.get_em()[1] == 2) {
        str += "cold ";
    }
    else {}

    if (game_tamagotchi.get_em()[2] == 0) {
        str += "noise ";
    }
    else {}

    if (game_tamagotchi.get_em()[3] == 2) {
        str += "tired ";
    }
    else {}

    if (game_tamagotchi.get_em()[4] == 0) {
        str += "glare ";
    }
    else if (game_tamagotchi.get_em()[4] == 2) {
        str += "dark ";
    }
    else {}

    if (game_tamagotchi.get_em()[5] == 2) {
        str += "sleepy ";
    }
    else {}

    if (game_tamagotchi.get_em()[6] == 0) {
        str += "attention ";
    }
    else {}

    if (game_tamagotchi.get_em()[7] == 2) {
        str += "hungry ";
    }

    if(game_tamagotchi.get_State() == SLEEP) {
        str += "Sleep ";
    }
    else {}

    return str;
}