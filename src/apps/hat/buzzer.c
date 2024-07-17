#include "buzzer.h"

void callback(void *data, uint8_t note, uint8_t volume)
{
    /** If below the valid note range */
    if (note < 60)
        pwm_stop(buzzer_pwm);
    else
    {
        pwm_start(buzzer_pwm);
        uint32_t frequency = 440.0 * pow(2.0, (note - 69.0) / 12.0);
        pwm_frequency_set(buzzer_pwm, frequency);
    }
}

void buzzer_init(void)
{
    piezo = piezo_init(&piezo_config);
    mmelody = mmelody_init(&mmelody_dev, BUZZER_UPDATE_RATE, callback, NULL);
    mmelody_speed_set(mmelody, 255);
    mmelody_play(mmelody, main_melody);

    buzzer_pwm = pwm_init(&buz_pwm_cfg);

    if (!buzzer_pwm)
        panic(LED_ERROR_PIO, 10);

    pwm_start(buzzer_pwm);
}

void buzzer_update(void)
{
    mmelody_update(mmelody);
}

void buzzer_beep(void)
{
    piezo_beep_long(piezo);
}

void buzzer_bumper_hit(void)
{
    mmelody_play(mmelody, bumper_hit_melody);
}