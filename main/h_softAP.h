#ifndef softAP_h
#define softAP_h

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define ESP_WIFI_SSID      "ESP32-SoftAP"
#define ESP_WIFI_PASS      "password"
#define ESP_WIFI_CHANNEL   1
#define MAX_STA_CONN       10

#endif