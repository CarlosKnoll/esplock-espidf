#ifndef SPIFFS_h
#define SPIFFS_h

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <esp_http_server.h>

#define CSS_PATH                "/spiffs/esplockstyle.css"

#define ACCESS_HTML_PATH        "/spiffs/esplockAccess.html"
#define ADD_USER_HTML_PATH      "/spiffs/esplockAddUser.html"
#define MAIN_HTML_PATH          "/spiffs/esplockMain.html"
#define NEW_ACCESS_HTML_PATH    "/spiffs/esplockNewAccess.html"
#define USERS_HTML_PATH         "/spiffs/esplockUsers.html"

#define ACCESS_JS_PATH          "/spiffs/esplockAccess.js"
#define ADD_USER_JS_PATH        "/spiffs/esplockAddUser.js"
#define MAIN_JS_PATH            "/spiffs/esplockMain.js"
#define NEW_ACCESS_JS_PATH      "/spiffs/esplockNewAccess.js"
#define USERS_JS_PATH           "/spiffs/esplockUsers.js"

#define TEST_SENSOR_PATH        "/spiffs/test.html"
#define TEST_SENSOR_JS_PATH     "/spiffs/test.js"

#define DATABASE_PATH           "/spiffs/users.db"

#endif