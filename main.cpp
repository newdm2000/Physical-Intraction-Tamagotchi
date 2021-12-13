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

bool is_daytime(int time) {
    int d_time;
    d_time = time - (TIME_DAYTIME+TIME_NIGHT)*(time/(TIME_DAYTIME+TIME_NIGHT));
    if(d_time >=0 && d_time < TIME_DAYTIME) return true;
    else return false;
}

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

void Emotion::set_Emote_Brightness(double _ilum) { 
    if(_ilum >= MAX_ILLUMINANCE) _Emote_Brightness = GLARE;
    else if(_ilum <= MIN_ILLUMINANCE) _Emote_Brightness = DARK;
    else _Emote_Brightness = BR_NORMAL;
}

void Emotion::set_Emote_Condition(int _time) {
    if(is_daytime(_time)) _Emote_Condition = CO_NORMAL;
    else _Emote_Condition = SLEEPY;
}

void Emotion::set_Emote_Attention(bool _shock, bool _slide) {
    if(_shock == true && _Emote_Attention == AT_NORMAL) _Emote_Attention = ATTENTION;
    else if(_slide == true && _Emote_Attention == ATTENTION) _Emote_Attention = AT_NORMAL;
    else if(_Emote_Attention == AT_NORMAL) _Emote_Attention = AT_NORMAL;
    else if(_Emote_Attention == ATTENTION) _Emote_Attention = ATTENTION;
    else _Emote_Attention = AT_NORMAL;
}

void Emotion::set_Emote_Hungry(int _time, bool _meal){
    if((((_time+1)%TIME_MEAL) == 0) && _Emote_Hungry == HG_NORMAL && is_daytime(_time)) _Emote_Hungry = HUNGRY;
    else if(_meal == true && _Emote_Hungry == HUNGRY) _Emote_Hungry = HG_NORMAL;
    else if(_Emote_Hungry == HG_NORMAL) _Emote_Hungry = HG_NORMAL;
    else if(_Emote_Hungry == HUNGRY) _Emote_Hungry = HUNGRY;
    else _Emote_Hungry = HG_NORMAL;
}

void Emotion::set_Emote_Normal() {
    if(
        _Emote_Humidity == HM_NORMAL &&
        _Emote_Temperature == TE_NORMAL &&
        _Emote_Noise == NO_NORMAL &&
        _Emote_Battery == BA_NORMAL &&
        _Emote_Brightness == BR_NORMAL &&
        _Emote_Attention == AT_NORMAL &&
        _Emote_Hungry == HG_NORMAL
    ) _Emote_Normal = true;
    else _Emote_Normal = false;
}

void Emotion::set_emotion(double _humid, double _tempr, int _noise, double _batt, double _ilum, int _time, bool _shock, bool _slide, bool _meal) {
    set_Emote_Humidity(_humid);
    set_Emote_Temperature(_tempr);
    set_Emote_Noise(_noise);
    set_Emote_Battery(_batt);
    set_Emote_Brightness(_ilum);
    set_Emote_Condition(_time);
    set_Emote_Attention(_shock, _slide);
    set_Emote_Hungry(_time, _meal);
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
    Serial.print("\t");
    Serial.print(_Emote_Normal);
    Serial.println("\n");
}

Tamagotchi::Tamagotchi(){
    _emote = Emotion();
    _state = NORMAL;
    Game_State = false;
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
    Game_State = false;
    _state = NORMAL;
    t_HP = 100;
    Game_step = 5;
    cnt1 = false;
}

void Tamagotchi::play(){
    if(Game_State == true){
        if(_emote.get_Emote_Normal() == true && _emote.get_Emote_Condition() == SLEEPY && _state == NORMAL) {
            _state = SLEEP;
        }
        else if(_emote.get_Emote_Condition() == CO_NORMAL && _state == SLEEP) {
            _state = NORMAL;
            Game_step++;
        }
        else if(_state == NORMAL) {
            _state == NORMAL;
             _emote.set_emotion(t_humid, t_tempr, t_noise, t_batt, t_ilum, t_time, t_shock, t_slide, t_meal);
        }
        else if(_state == SLEEP) {
            _state == SLEEP;
            _emote.set_Emote_Condition(t_time);
        }
        else _state == NORMAL;



        if(!(_emote.get_Emote_Normal()) && critical_time == -1 && !(cnt1)) {
            critical_time = t_time;
        }
        else if(!(_emote.get_Emote_Normal()) && ((t_time - critical_time) % LIMIT_TIME == 0) && (t_time != critical_time) && !(cnt1)) {
            cnt1 = true;
            t_HP = t_HP - 5;
        }
        else if(!(_emote.get_Emote_Normal()) && ((t_time - critical_time) % LIMIT_TIME != 0) && (t_time != critical_time) && (cnt1)) {
            cnt1 = false;
        }
        else if((_emote.get_Emote_Normal())) {
            cnt1 = false;
            critical_time = -1;
        }
        else if(cnt1) cnt1 = true;
        else cnt1 = false;

        if(t_HP<=0){
            Game_State = false;
        }
        if (critical_time != -1) Serial.println(t_time - critical_time);
    }
    Serial.println(t_HP);
    set_em();
    _emote.print_Emote();
}

void Tamagotchi::set_val_sensor(double _humid, double _tempr, int _noise, double _batt, double _ilum, int _time, bool _shock, bool _slide, bool _meal) 
{
    t_humid = _humid;
    t_tempr = _tempr;
    t_noise = _noise;
    t_batt = _batt;
    t_ilum = _ilum;
    t_time = _time;
    t_shock = _shock;
    t_slide = _slide;
    t_meal = _meal;
}

void Tamagotchi::set_em(){
    em[0] = _emote.get_Emote_Humidity();
    em[1] = _emote.get_Emote_Temperature();
    em[2] = _emote.get_Emote_Noise();
    em[3] = _emote.get_Emote_Battery();
    em[4] = _emote.get_Emote_Brightness();
    em[5] = _emote.get_Emote_Condition();
    em[6] = _emote.get_Emote_Attention();
    em[7] = _emote.get_Emote_Hungry();
    em[8] = _emote.get_Emote_Normal();
}