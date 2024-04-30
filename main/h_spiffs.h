#ifndef spiffs_h
#define spiffs_h

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <esp_http_server.h>

esp_err_t send_web_page(httpd_req_t *req, char* PATH);

#endif