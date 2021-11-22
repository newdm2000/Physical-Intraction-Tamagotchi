#include "Arduino.h"
#include "main.h"
#include "setting.h"

/*  Emote_Humidity 
    Emote_Temperature 
    Emote_Noise 
    Emote_Battery 
    Emote_Brightness 
    Emote_Condition 
    Emote_Attention 
    Emote_Hungry 
*/

Emotion::Emotion(){
    Happy = true;
    Sleep = false;
    _Emote_Humidity = HM_NORMAL;
    _Emote_Temperature = TE_NORMAL;
    _Emote_Noise = NO_NORMAL;
    _Emote_Battery = BA_NORMAL;
    _Emote_Brightness = BR_NORMAL;
    _Emote_Condition = CO_NORMAL;
    _Emote_Attention = AT_NORMAL;
    _Emote_Hungry = HG_NORMAL;
}

void Emotion::set_Emote_Humidity(double _humid) {
    if(_humid>=MAX_HUMINITY) _Emote_Humidity = WET;
    else if (_humid<=MIN_HUMINITY) _Emote_Humidity = DRY;
    else _Emote_Humidity = HM_NORMAL;
}

void Emotion::set_Emote_Temperature(double _tempr) {
    if(_tempr >= MAX_TEMPERATURE) _Emote_Temperature = HOT;
    else if(_tempr <= MIN_TEMPERATURE) _Emote_Temperature = COLD;
    else _Emote_Temperature = TE_NORMAL;
}

void Emotion::set_Emote_Noise(int _noise) {
    if(_noise >= MAX_SOUND) _Emote_Noise = NOISE;
    else _Emote_Noise = NO_NORMAL;
}

void Emotion::set_Emote_Battery(double _batt) {
    if(_batt <= MIN_BATTERY) _Emote_Battery = TIRED;
    else _Emote_Battery = BA_NORMAL;
}

void Emotion::set_Emote_Brightness(int _ilum) { 
    if(_ilum >= MAX_ILLUMINANCE) _Emote_Brightness = GLARE;
    else if(_ilum <= MIN_ILLUMINANCE) _Emote_Brightness = DARK;
    else _Emote_Brightness = BR_NORMAL;
}

//void Emotion::set_Emote_Condition();

void Emotion::set_Emote_Attention(bool _shock, bool _slide) {
    if(_shock == true && _Emote_Attention == AT_NORMAL) _Emote_Attention = ATTENTION;
    else if(_slide == true && _Emote_Attention == ATTENTION) _Emote_Attention = AT_NORMAL;
    else if(_Emote_Attention == AT_NORMAL) _Emote_Attention = AT_NORMAL;
    else if(_Emote_Attention == ATTENTION) _Emote_Attention = ATTENTION;
    else _Emote_Attention = AT_NORMAL;
}