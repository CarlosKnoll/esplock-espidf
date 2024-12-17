#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"
#include "font8x8_basic.h"

#define CONFIG_SDA_GPIO 4
#define CONFIG_SCL_GPIO 15
#define CONFIG_RESET_GPIO 16