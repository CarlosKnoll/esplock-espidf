#ifndef Webserver_h
#define Webserver_h

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>
#include "h_SPIFFS.h"

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

// /* Scratch buffer size */
// #define SCRATCH_BUFSIZE  8192

// struct file_server_data {
//     /* Base path of file storage */
//     char base_path[10];

//     /* Scratch buffer for temporary storage during file transfer */
//     char scratch[SCRATCH_BUFSIZE];
// };

char index_html[4096];
char final_html[4096];
char response_data[4096];

esp_err_t send_web_page(httpd_req_t *req, char* PATH);
//esp_err_t download_get_handler(httpd_req_t *req)

static esp_err_t css_handler(httpd_req_t *req)          { return send_web_page(req, CSS_PATH); }

static esp_err_t acessos_handler(httpd_req_t *req)      { return send_web_page(req, ACCESS_HTML_PATH);}
static esp_err_t addUser_handler(httpd_req_t *req)      { return send_web_page(req, ADD_USER_HTML_PATH);}
static esp_err_t main_handler(httpd_req_t *req)         { return send_web_page(req, MAIN_HTML_PATH); }
static esp_err_t newAccess_handler(httpd_req_t *req)    { return send_web_page(req, NEW_ACCESS_HTML_PATH);}
static esp_err_t users_handler(httpd_req_t *req)        { return send_web_page(req, USERS_HTML_PATH);}

static esp_err_t js_acessos_handler(httpd_req_t *req)      { return send_web_page(req, ACCESS_JS_PATH);}
static esp_err_t js_addUser_handler(httpd_req_t *req)      { return send_web_page(req, ADD_USER_JS_PATH);}
static esp_err_t js_main_handler(httpd_req_t *req)         { return send_web_page(req, MAIN_JS_PATH); }
static esp_err_t js_newAccess_handler(httpd_req_t *req)    { return send_web_page(req, NEW_ACCESS_JS_PATH);}
static esp_err_t js_users_handler(httpd_req_t *req)        { return send_web_page(req, USERS_JS_PATH);}

static esp_err_t sensor_test_handler(httpd_req_t *req)          { return send_web_page(req, TEST_SENSOR_PATH);}
static esp_err_t js_sensor_test_hanlder(httpd_req_t *req)       { return send_web_page(req, TEST_SENSOR_JS_PATH);}

//static esp_err_t db_handler(httpd_req_t *req)                   { return download_get_handler(req);}

esp_err_t ws_handler(httpd_req_t *req);


static const httpd_uri_t ws = {
        .uri        = "/ws",
        .method     = HTTP_GET,
        .handler    = ws_handler,
        .user_ctx   = NULL,
        .is_websocket = true
};

static const httpd_uri_t esplock_css = {
        .uri        = "/esplockstyle.css",
        .method     = HTTP_GET,
        .handler    = css_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_acessos = {
        .uri        = "/acessos",
        .method     = HTTP_GET,
        .handler    = acessos_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_novoUsuario = {
        .uri        = "/novoUsuario",
        .method     = HTTP_GET,
        .handler    = addUser_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_main = {
        .uri        = "/",
        .method     = HTTP_GET,
        .handler    = main_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_novoAcesso = {
        .uri        = "/entrada",
        .method     = HTTP_GET,
        .handler    = newAccess_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_usuarios = {
        .uri        = "/usuarios",
        .method     = HTTP_GET,
        .handler    = users_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_js_acessos = {
        .uri        = "/acessos.js",
        .method     = HTTP_GET,
        .handler    = js_acessos_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_js_novoUsuario = {
        .uri        = "/novoUsuario.js",
        .method     = HTTP_GET,
        .handler    = js_addUser_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_js_main = {
        .uri        = "/main.js",
        .method     = HTTP_GET,
        .handler    = js_main_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_js_novoAcesso = {
        .uri        = "/entrada.js",
        .method     = HTTP_GET,
        .handler    = js_newAccess_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t esplock_js_usuarios = {
        .uri        = "/usuarios.js",
        .method     = HTTP_GET,
        .handler    = js_users_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};



static const httpd_uri_t sensor_test = {
        .uri        = "/test",
        .method     = HTTP_GET,
        .handler    = sensor_test_handler,
        .user_ctx   = NULL,
        .is_websocket = false
};

static const httpd_uri_t sensor_test_js = {
        .uri        = "/test.js",
        .method     = HTTP_GET,
        .handler    = js_sensor_test_hanlder,
        .user_ctx   = NULL,
        .is_websocket = false
};


// static const httpd_uri_t database = {
//         .uri        = "/users.db",
//         .method     = HTTP_GET,
//         .handler    = db_handler,
//         .user_ctx   = server_data,
//         .is_websocket = false
// };

#endif