#include "sound.h"

void Sound::highClick(){
    system("aplay -q sfx/high_click.wav > /dev/null 2>&1 &");
}

void Sound::lowClick(){
    system("aplay -q sfx/low_click.wav > /dev/null 2>&1 &");
}

void Sound::clickSwish(){
    system("aplay -q sfx/click_swish.wav > /dev/null 2>&1 &");
}

void Sound::tpLowHealth(){
    system("aplay -q sfx/tp_low_health.wav > /dev/null 2>&1 &");

}

void Sound::tpLettersRead(){
    system("aplay -q sfx/tp_letters_read.wav > /dev/null 2>&1 &"); 
}

void Sound::tpFanfareGoldenBug(){
    system("aplay -q sfx/tp_fanfare_goldenbug.wav > /dev/null 2>&1 &"); 
}

void Sound::typewriterKey(){
    system("aplay -q sfx/typewriter_key.wav > /dev/null 2>&1 &"); 
}

void Sound::smolClick(){
    system("aplay -q sfx/smol_click.wav > /dev/null 2>&1 &"); 
}

void Sound::lightSwitch(){
    system("aplay -q sfx/light_switch.wav > /dev/null 2>&1 &"); 
}


