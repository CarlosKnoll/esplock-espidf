#include <h_DeepSleep.h>

static void calibrate_touch_pad(touch_pad_t pad)
{
    touch_pad_config(pad, 1000);

    int avg = 0;
    const size_t sample_count = 128;
    for (int i = 0; i < sample_count; ++i) {
        uint16_t val;
        touch_pad_read(pad, &val);
        avg += val;
    }
    avg /= sample_count;
    const int min_reading = 300;
    if (avg < min_reading) {
        printf("Average reading is too low: %d (expecting at least %d). ", avg, min_reading);
        touch_pad_config(pad, 0);
    } else {
        int threshold = avg - 100;
        printf("Average: %d, wakeup threshold set to %d.\n", avg, threshold);
        touch_pad_config(pad, threshold);
    }
}

void deepSleep_init(){
    touch_pad_init();
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_0V); 
    calibrate_touch_pad(TOUCH_PAD_NUM2);
    esp_sleep_enable_touchpad_wakeup();
}