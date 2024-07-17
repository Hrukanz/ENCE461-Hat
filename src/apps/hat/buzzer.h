#ifndef BUZZER_H
#define BUZZER_H

#include "piezo.h"
#include "piezo_beep.h"
#include "target.h"
#include "mmelody.h"
#include "pwm.h"
#include "panic.h"
#include "math.h"

#define TUNE_BPM 100
#define PWM_FREQ_HZ 1000
#define DF1 "G/A/GF#E/"
#define DF2 "EF#G/A5/B4/A5/GF#E/ "
#define DF3 " D#EF#/G5/A4/G5/F#ED#/"
#define DUEL_FATES DF1 DF1 DF1 DF1 " " DF2 DF2 DF3 DF3 "/ "
#define MARIO_DEATH "B5F5/ F5E5D5C5E4E4C4"
#define BUZZER_UPDATE_RATE 1000
#define LED_TAPE_UPDATE_RATE 25

static const piezo_cfg_t piezo_config =
    {
        .pio = BUZZ_PIN};

static piezo_t piezo;

static mmelody_obj_t mmelody_dev;

static mmelody_t mmelody;

static pwm_t buzzer_pwm;

static const char main_melody[] = DUEL_FATES ":";
static const char bumper_hit_melody[] = MARIO_DEATH "             <" DUEL_FATES ">";

static const pwm_cfg_t buz_pwm_cfg =
    {
        .pio = BUZZ_PIN,
        .period = PWM_PERIOD_DIVISOR(PWM_FREQ_HZ),
        .duty = PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50),
        .align = PWM_ALIGN_LEFT,
        .polarity = PWM_POLARITY_HIGH,
        .stop_state = PIO_OUTPUT_LOW
    };

void buzzer_init(void);

void buzzer_beep(void);

void buzzer_bumper_hit(void);

void buzzer_update(void);

#endif