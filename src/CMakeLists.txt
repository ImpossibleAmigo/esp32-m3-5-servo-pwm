idf_component_register(
    SRCS "main.c" "potentiometer.c" "servo.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_adc
)