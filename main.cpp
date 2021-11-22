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
    _Emote_Humidity = HM_NORMAL;
    _Emote_Temperature = TE_NORMAL;
    _Emote_Noise = NO_NORMAL;
    _Emote_Battery = BA_NORMAL;
    _Emote_Brightness = BR_NORMAL;
    _Emote_Condition = CO_NORMAL;
    _Emote_Attention = AT_NORMAL;
    _Emote_Hungry = HG_NORMAL;
    _Emote_Normal = true;
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

void Emotion::set_Emote_Condition();

void Emotion::set_Emote_Attention(bool _shock, bool _slide) {
    if(_shock == true && _Emote_Attention == AT_NORMAL) _Emote_Attention = ATTENTION;
    else if(_slide == true && _Emote_Attention == ATTENTION) _Emote_Attention = AT_NORMAL;
    else if(_Emote_Attention == AT_NORMAL) _Emote_Attention = AT_NORMAL;
    else if(_Emote_Attention == ATTENTION) _Emote_Attention = ATTENTION;
    else _Emote_Attention = AT_NORMAL;
}

void set_Emote_Hungry();

void Emotion::set_Emote_Normal() {
    if(
        _Emote_Humidity == HM_NORMAL &&
        _Emote_Temperature == TE_NORMAL &&
        _Emote_Noise == NO_NORMAL &&
        _Emote_Battery == BA_NORMAL &&
        _Emote_Brightness == BR_NORMAL &&
        //_Emote_Condition == CO_NORMAL &&
        _Emote_Attention == AT_NORMAL &&
        _Emote_Hungry == HG_NORMAL
    ) _Emote_Normal = true;
    else _Emote_Normal = false;
}

void Emotion::set_emotion(double _humid, double _tempr, int _noise, double _batt, int _ilum, bool _shock, bool _slide) {
    set_Emote_Humidity(_humid);
    set_Emote_Temperature(_tempr);
    set_Emote_Noise(_noise);
    set_Emote_Battery(_batt);
    set_Emote_Brightness(_ilum);
    set_Emote_Condition();
    set_Emote_Attention(_shock, _slide);
    set_Emote_Hungry();
    set_Emote_Normal();
}

void Emotion::print_Emote() {
    Serial.print(_Emote_Humidity);
    Serial.print("\t");
    Serial.print(_Emote_Temperature);
    Serial.print("\t");
    Serial.print(_Emote_Noise);
    Serial.print("\t");
    Serial.print(_Emote_Battery);
    Serial.print("\t");
    Serial.print(_Emote_Brightness);
    Serial.print("\t");
    Serial.print(_Emote_Condition);
    Serial.print("\t");
    Serial.print(_Emote_Attention);
    Serial.print("\t");
    Serial.print(_Emote_Hungry);
    Serial.println("\n");
}

Tamagotchi::Tamagotchi(){
    _emote = Emotion();
    _state = NORMAL;
    Game_State = true;
    t_humid = 0;
    t_tempr = 0;
    t_noise = 0;
    t_batt = 0;
    t_ilum = 0;
    t_shock = false;
    t_slide = true;
    Game_step = 0;
    t_HP = 100;
}

void Tamagotchi::start_game(){
    _emote = Emotion();
    Game_State = true;
    _state = NORMAL;
    t_HP = 100;
    Game_step = 0;
}

void Tamagotchi::play(){
    if(Game_State == true){
        _emote.set_emotion(t_humid, t_tempr, t_noise, t_ilum, t_batt, t_shock, t_slide);
        if(_emote.get_Emote_Normal() == true && _emote.get_Emote_Condition() == SLEEPY && _state == NORMAL) {
            _state = SLEEP;
        }
        else if(_emote.get_Emote_Condition() == CO_NORMAL && _state == SLEEP) {
            _state = NORMAL;
            Game_step++;
        }
        else if(_state == NORMAL) _state == NORMAL;
        else if(_state == SLEEP) _state == SLEEP;
        else _state == NORMAL;

        if(t_HP<=0){
            Game_State == false;
        }
    }

    _emote.print_Emote();
}