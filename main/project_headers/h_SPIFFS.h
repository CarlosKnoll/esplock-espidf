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

#define USERS_DATA_PATH         "/spiffs/users"
#define HISTORY_DATA_PATH       "/spiffs/history"

esp_err_t send_web_page(httpd_req_t *req, char* PATH);

#endif