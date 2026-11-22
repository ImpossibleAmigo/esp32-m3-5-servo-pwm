#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "potentiometer.h"
#include "servo.h"

static const char *TAG = "M3_5_CTRL";

#define LOOP_DELAY_MS          20     // 50 Гц — синхронно з періодом PWM 20 мс
#define SERVO_DEADBAND_DEG     1.0f   // Мертва зона від шумів АЦП
#define CLIP_LOG_STEP_DEG      5.0f   // Крок логування ручки під час обрізки діапазону

int app_main(void)
{
    ESP_ERROR_CHECK(pot_init());
    ESP_ERROR_CHECK(servo_init());

    float prev_servo_angle = -100.0f;
    float prev_clipped_pot_angle = -100.0f;

    ESP_LOGI(TAG, "Started. Pot max: %.1f deg, Servo max: %.1f deg (1:1 ratio, clipped)",
             POT_MAX_ANGLE_DEG, SERVO_MAX_ANGLE_DEG);

    while (1) {
        int raw = 0;
        ESP_ERROR_CHECK(pot_read_raw(&raw));

        float pot_angle = pot_raw_to_angle(raw);
        float servo_angle = pot_angle;
        bool clipped = false;

        if (servo_angle < 0.0f) {
            servo_angle = 0.0f;
            clipped = true;
        } else if (servo_angle > SERVO_MAX_ANGLE_DEG) {
            servo_angle = SERVO_MAX_ANGLE_DEG;
            clipped = true;
        }

        bool servo_changed = fabsf(servo_angle - prev_servo_angle) >= SERVO_DEADBAND_DEG;
        bool clip_step_reached = clipped &&
                                 (fabsf(pot_angle - prev_clipped_pot_angle) >= CLIP_LOG_STEP_DEG);

        if (servo_changed) {
            ESP_ERROR_CHECK(servo_set_angle(servo_angle));
            prev_servo_angle = servo_angle;
            prev_clipped_pot_angle = pot_angle;

            ESP_LOGI(TAG, "Left offset: %.1f deg | Pot: %.1f deg (ADC: %d)%s",
                     servo_angle, pot_angle, raw, clipped ? " [CLIPPED]" : "");
        } else if (clip_step_reached) {
            prev_clipped_pot_angle = pot_angle;

            ESP_LOGI(TAG, "Left offset: %.1f deg | Pot: %.1f deg (ADC: %d) [CLIPPED]",
                     servo_angle, pot_angle, raw);
        }

        vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
    }
}