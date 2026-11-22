#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <stdint.h>
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

#define POT_ADC_UNIT        ADC_UNIT_1
#define POT_ADC_CHANNEL     ADC_CHANNEL_3   // GPIO4 на ESP32-S3
#define POT_ADC_MAX_RAW     4095.0f
#define POT_MAX_ANGLE_DEG   270.0f          // Фізичний кут повороту потенціометра
#define POT_AVG_SAMPLES     16              // Кількість вибірок для згладжування

esp_err_t pot_init(void);
esp_err_t pot_read_raw(int *out_raw);
float pot_raw_to_angle(int raw);

#endif // POTENTIOMETER_H