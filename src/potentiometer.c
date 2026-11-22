#include "potentiometer.h"

static adc_oneshot_unit_handle_t s_adc_handle = NULL;

esp_err_t pot_init(void)
{
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = POT_ADC_UNIT,
    };
    esp_err_t err = adc_oneshot_new_unit(&init_cfg, &s_adc_handle);
    if (err != ESP_OK) {
        return err;
    }

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten    = ADC_ATTEN_DB_12,
    };
    return adc_oneshot_config_channel(s_adc_handle, POT_ADC_CHANNEL, &chan_cfg);
}

esp_err_t pot_read_raw(int *out_raw)
{
    if (s_adc_handle == NULL || out_raw == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    int sum = 0;
    for (int i = 0; i < POT_AVG_SAMPLES; i++) {
        int sample = 0;
        esp_err_t err = adc_oneshot_read(s_adc_handle, POT_ADC_CHANNEL, &sample);
        if (err != ESP_OK) {
            return err;
        }
        sum += sample;
    }

    *out_raw = sum / POT_AVG_SAMPLES;
    return ESP_OK;
}

float pot_raw_to_angle(int raw)
{
    if (raw < 0) {
        raw = 0;
    } else if (raw > (int)POT_ADC_MAX_RAW) {
        raw = (int)POT_ADC_MAX_RAW;
    }
    return ((float)raw / POT_ADC_MAX_RAW) * POT_MAX_ANGLE_DEG;
}