#ifndef main_h
#define main_h

#include "math.h"
#include "Arduino.h"


enum Emote_Humidity {
    WET,
    HM_NORMAL,
    DRY
};

enum Emote_Temperature {
    HOT,
    TE_NORMAL,
    COLD
};

enum Emote_Noise {
    NOISE,
    NO_NORMAL
};

enum Emote_Battery {
    BA_NORMAL,
    TIRED
};

enum Emote_Brightness {
    GLARE,
    BR_NORMAL,
    DARK
};

enum Emote_Condition {
    CO_NORMAL,
    SLEEPY
};

enum Emote_Attention {
    ATTENTION,
    AT_NORMAL
};

enum Emote_Hungry {
    HG_NORMAL,
    HUNGRY
};

enum State {
    NORMAL,
    SLEEP
};


class Emotion {
    public:
        ~Emotion();
        Emotion();
        void set_Emote_Humidity(double _humid);
        Emote_Humidity get_Emote_Humidity() { return _Emote_Humidity; }
        void set_Emote_Temperature(double _tempr);
        Emote_Temperature get_Emote_Temperature() { return _Emote_Temperature; }
        void set_Emote_Noise(int _noise);
        Emote_Noise get_Emote_Noise() { return _Emote_Noise; }
        void set_Emote_Battery(double _batt);
        Emote_Battery get_Emote_Battery() { return _Emote_Battery; }
        void set_Emote_Brightness(int _ilum);
        Emote_Brightness get_Emote_Brightness() { return _Emote_Brightness; }
        void set_Emote_Condition();
        Emote_Condition get_Emote_Condition() { return _Emote_Condition; }
        void set_Emote_Attention(bool _shock, bool _slide);
        Emote_Attention get_Emote_Attention() { return _Emote_Attention; }
        void set_Emote_Hungry();
        Emote_Hungry get_Emote_Hungry() { return _Emote_Hungry; }
        void set_Emote_Normal();
        bool get_Emote_Normal() { return _Emote_Normal; };
        
        void set_emotion(double _humid, double _tempr, int _noise, double _batt, int _ilum, bool _shock, bool _slide);
        void print_Emote();

    private:
        Emote_Humidity _Emote_Humidity;
        Emote_Temperature _Emote_Temperature;
        Emote_Noise _Emote_Noise;
        Emote_Battery _Emote_Battery;
        Emote_Brightness _Emote_Brightness;
        Emote_Condition _Emote_Condition;
        Emote_Attention _Emote_Attention;
        Emote_Hungry _Emote_Hungry;
        bool _Emote_Normal;
};


class Tamagotchi{
    public:
        ~Tamagotchi();
        Tamagotchi();
        void set_State(State __state) { _state = __state; };
        State get_State() { return _state; }
        void start_game();
        void play();
        void set_val_sensor(double, double, double, int, int, bool, bool);
        
    private:
        bool Game_State;
        State _state;
        Emotion _emote;
        int Game_step;
        int t_HP;

        //sensor
        double t_humid;
        double t_tempr;
        int t_noise;
        double t_batt;
        int t_ilum;
        bool t_shock;
        bool t_slide;  
};


#endif /*main.h*/