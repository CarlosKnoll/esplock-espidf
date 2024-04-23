/* WebSocket Echo Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <webServer.h>
#include <spiffs.h>
#include <directories.h>

static char *SERVER_TAG = "ws_echo_server";

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

/* A simple example that demonstrates using websocket echo server
 */

/*
 * Structure holding server handle
 * and internal socket fd in order
 * to use out of request send
 */
// struct async_resp_arg {
//     httpd_handle_t hd;
//     int fd;
// };

// /*
//  * async send function, which we put into the httpd work queue
//  */
// static void ws_async_send(void *arg)
// {
//     static const char * data = "Async data";
//     struct async_resp_arg *resp_arg = arg;
//     httpd_handle_t hd = resp_arg->hd;
//     int fd = resp_arg->fd;
//     httpd_ws_frame_t ws_pkt;
//     memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
//     ws_pkt.payload = (uint8_t*)data;
//     ws_pkt.len = strlen(data);
//     ws_pkt.type = HTTPD_WS_TYPE_TEXT;

//     httpd_ws_send_frame_async(hd, fd, &ws_pkt);
//     free(resp_arg);
// }

// static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req)
// {
//     struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
//     if (resp_arg == NULL) {
//         return ESP_ERR_NO_MEM;
//     }
//     resp_arg->hd = req->handle;
//     resp_arg->fd = httpd_req_to_sockfd(req);
//     esp_err_t ret = httpd_queue_work(handle, ws_async_send, resp_arg);
//     if (ret != ESP_OK) {
//         free(resp_arg);
//     }
//     return ret;
// }

/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */
// static esp_err_t echo_handler(httpd_req_t *req)
// {
//     if (req->method == HTTP_GET) {
//         ESP_LOGI(SERVER_TAG, "Handshake done, the new connection was opened");
//         return ESP_OK;
//     }
//     httpd_ws_frame_t ws_pkt;
//     uint8_t *buf = NULL;
//     memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
//     ws_pkt.type = HTTPD_WS_TYPE_TEXT;
//     /* Set max_len = 0 to get the frame len */
//     esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
//     if (ret != ESP_OK) {
//         ESP_LOGE(SERVER_TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
//         return ret;
//     }
//     ESP_LOGI(SERVER_TAG, "frame len is %d", ws_pkt.len);
//     if (ws_pkt.len) {
//         /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
//         buf = calloc(1, ws_pkt.len + 1);
//         if (buf == NULL) {
//             ESP_LOGE(SERVER_TAG, "Failed to calloc memory for buf");
//             return ESP_ERR_NO_MEM;
//         }
//         ws_pkt.payload = buf;
//         /* Set max_len = ws_pkt.len to get the frame payload */
//         ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
//         if (ret != ESP_OK) {
//             ESP_LOGE(SERVER_TAG, "httpd_ws_recv_frame failed with %d", ret);
//             free(buf);
//             return ret;
//         }
//         ESP_LOGI(SERVER_TAG, "Got packet with message: %s", ws_pkt.payload);
//     }
//     ESP_LOGI(SERVER_TAG, "Packet type: %d", ws_pkt.type);
//     if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
//         strcmp((char*)ws_pkt.payload,"Trigger async") == 0) {
//         free(buf);
//         return trigger_async_send(req->handle, req);
//     }

//     ret = httpd_ws_send_frame(req, &ws_pkt);
//     if (ret != ESP_OK) {
//         ESP_LOGE(SERVER_TAG, "httpd_ws_send_frame failed with %d", ret);
//     }
//     free(buf);
//     return ret;
// }


// static const httpd_uri_t ws = {
//         .uri        = "/ws",
//         .method     = HTTP_GET,
//         .handler    = echo_handler,
//         .user_ctx   = NULL,
//         .is_websocket = true
// };



static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 15;

    // Start the httpd server
    ESP_LOGI(SERVER_TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Registering the ws handler
        ESP_LOGI(SERVER_TAG, "Registering URI handlers");
        //httpd_register_uri_handler(server, &ws);
        httpd_register_uri_handler(server, &esplock_css);
        httpd_register_uri_handler(server, &esplock_acessos);
        httpd_register_uri_handler(server, &esplock_novoUsuario);
        httpd_register_uri_handler(server, &esplock_main);
        httpd_register_uri_handler(server, &esplock_novoAcesso);
        httpd_register_uri_handler(server, &esplock_usuarios);
        httpd_register_uri_handler(server, &esplock_js_acessos);
        httpd_register_uri_handler(server, &esplock_js_novoUsuario);
        httpd_register_uri_handler(server, &esplock_js_main);
        httpd_register_uri_handler(server, &esplock_js_novoAcesso);
        httpd_register_uri_handler(server, &esplock_js_usuarios);

        return server;
    }

    ESP_LOGI(SERVER_TAG, "Error starting server!");
    return NULL;
}



// 

// static esp_err_t stop_webserver(httpd_handle_t server)
// {
//     // Stop the httpd server
//     return httpd_stop(server);
// }

// static void disconnect_handler(void* arg, esp_event_base_t event_base,
//                                int32_t event_id, void* event_data)
// {
//     httpd_handle_t* server = (httpd_handle_t*) arg;
//     if (*server) {
//         ESP_LOGI(SERVER_TAG, "Stopping webserver");
//         if (stop_webserver(*server) == ESP_OK) {
//             *server = NULL;
//         } else {
//             ESP_LOGE(SERVER_TAG, "Failed to stop http server");
//         }
//     }
// }

// static void connect_handler(void* arg, esp_event_base_t event_base,
//                             int32_t event_id, void* event_data)
// {
//     httpd_handle_t* server = (httpd_handle_t*) arg;
//     if (*server == NULL) {
//         ESP_LOGI(SERVER_TAG, "Starting webserver");
//         *server = start_webserver();
//     }
// }