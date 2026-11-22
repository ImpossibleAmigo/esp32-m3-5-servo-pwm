#ifndef SERVO_H
#define SERVO_H

#include "esp_err.h"
#include "driver/ledc.h"

#define SERVO_GPIO              5
#define SERVO_LEDC_MODE         LEDC_LOW_SPEED_MODE
#define SERVO_LEDC_TIMER        LEDC_TIMER_0
#define SERVO_LEDC_CHANNEL      LEDC_CHANNEL_0

#define SERVO_PWM_FREQ_HZ       50
#define SERVO_PWM_PERIOD_US     20000.0f
#define SERVO_PWM_RES_BITS      14
#define SERVO_PWM_RES           LEDC_TIMER_14_BIT

#define SERVO_MIN_PULSE_US      500.0f      // 0° (при дзижчанні на упорі підкоригуй до 550-600)
#define SERVO_MAX_PULSE_US      2500.0f     // 180° (при потребі підкоригуй до 2400)
#define SERVO_MAX_ANGLE_DEG     180.0f

esp_err_t servo_init(void);
esp_err_t servo_set_angle(float angle_deg);

#endif // SERVO_H