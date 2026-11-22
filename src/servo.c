#include "servo.h"

static uint32_t angle_to_duty(float angle_deg)
{
    if (angle_deg < 0.0f) {
        angle_deg = 0.0f;
    } else if (angle_deg > SERVO_MAX_ANGLE_DEG) {
        angle_deg = SERVO_MAX_ANGLE_DEG;
    }

    float pulse_us = SERVO_MIN_PULSE_US +
                     (angle_deg / SERVO_MAX_ANGLE_DEG) * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US);

    return (uint32_t)((pulse_us * (1U << SERVO_PWM_RES_BITS)) / SERVO_PWM_PERIOD_US);
}

esp_err_t servo_init(void)
{
    ledc_timer_config_t timer_cfg = {
        .speed_mode      = SERVO_LEDC_MODE,
        .duty_resolution = SERVO_PWM_RES,
        .timer_num       = SERVO_LEDC_TIMER,
        .freq_hz         = SERVO_PWM_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    esp_err_t err = ledc_timer_config(&timer_cfg);
    if (err != ESP_OK) {
        return err;
    }

    ledc_channel_config_t channel_cfg = {
        .gpio_num   = SERVO_GPIO,
        .speed_mode = SERVO_LEDC_MODE,
        .channel    = SERVO_LEDC_CHANNEL,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = SERVO_LEDC_TIMER,
        .duty       = 0,
        .hpoint     = 0,
    };
    return ledc_channel_config(&channel_cfg);
}

esp_err_t servo_set_angle(float angle_deg)
{
    uint32_t duty = angle_to_duty(angle_deg);
    esp_err_t err = ledc_set_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL, duty);
    if (err != ESP_OK) {
        return err;
    }
    return ledc_update_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL);
}