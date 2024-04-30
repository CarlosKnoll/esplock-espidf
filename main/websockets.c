#include <h_webServer.h>
#include <h_spiffs.h>
#include <h_spiffsDir.h>

static char *WS_TAG = "ws_server";
char * data;

/*
 * Structure holding server handle
 * and internal socket fd in order
 * to use out of request send
 */
struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
};

// /*
//  * async send function, which we put into the httpd work queue
//  */

static void response(void *arg)
{
    struct async_resp_arg *resp_arg = arg;
    httpd_handle_t hd = resp_arg->hd;
    int fd = resp_arg->fd;
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t*)data;
    ws_pkt.len = strlen(data);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    free(resp_arg);
}

/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */

static esp_err_t GENERIC_HANDLER(httpd_handle_t handle, httpd_req_t *req){
    struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
    if (resp_arg == NULL) {
        return ESP_ERR_NO_MEM;
    }
    resp_arg->hd = req->handle;
    resp_arg->fd = httpd_req_to_sockfd(req);
    esp_err_t ret = httpd_queue_work(handle, response, resp_arg);
    if (ret != ESP_OK) {
        free(resp_arg);
    }
    return ret;
}


static esp_err_t identifyPacket(httpd_ws_frame_t ws_pkt, httpd_req_t *req){
    //Check for message from users webpage
    //Current state: response is an identifier unique to each possible websocket message received
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strstr((char*)ws_pkt.payload,"populateUsers") != NULL) {
        data = "Usuarios: Popular tabela";
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strstr((char*)ws_pkt.payload,"removeUser") != NULL) {
        data = "Usuarios: Remover usuario";
        return GENERIC_HANDLER(req->handle, req);
    }

    //Check for message from add users webpage
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strstr((char*)ws_pkt.payload,"addUser") != NULL) {
        data = "Usuarios: Adicionar usuario";
        return GENERIC_HANDLER(req->handle, req);
    }
    
    //Check for message from access webpage    
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strstr((char*)ws_pkt.payload,"populateAccess") != NULL) { //Functional test response -> onload access webpage
        data = "Acessos: Popular tabela";
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char*)ws_pkt.payload,"clear") == 0) {
        data = "Acessos: Limpar DB";
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char*)ws_pkt.payload,"get") == 0) {
        data = "Acessos: Download DB";
        return GENERIC_HANDLER(req->handle, req);
    }

    //Check for message from RFID readings
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && (
        strcmp((char*)ws_pkt.payload,"readRFID") == 0 || strcmp((char*)ws_pkt.payload,"accessRFID") == 0 )) {
        data = "Lendo tag RFID manualmente";
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char*)ws_pkt.payload,"cancelRFID") == 0) {
        data = "cancel"; // Cancel manual RFID read request
        return GENERIC_HANDLER(req->handle, req);
    }

    //Check for message to update time
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strstr((char*)ws_pkt.payload,"epoch") != NULL) {
        // update internal rtc?
        ESP_LOGI(WS_TAG, "Should somehow update internal clock.");
        return ESP_OK; //No return needed for time ws message
    }

    else{
        return ESP_FAIL;
    }
}

esp_err_t ws_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET) {
        ESP_LOGI(WS_TAG, "Handshake done, the new connection was opened");
        return ESP_OK;
    }
    httpd_ws_frame_t ws_pkt;
    uint8_t *buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    /* Set max_len = 0 to get the frame len */
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(WS_TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }
    ESP_LOGI(WS_TAG, "frame len is %d", ws_pkt.len);
    if (ws_pkt.len) {
        /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
        buf = calloc(1, ws_pkt.len + 1);
        if (buf == NULL) {
            ESP_LOGE(WS_TAG, "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        /* Set max_len = ws_pkt.len to get the frame payload */
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK) {
            ESP_LOGE(WS_TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        ESP_LOGI(WS_TAG, "Got packet with message: %s", ws_pkt.payload);
    }
    ESP_LOGI(WS_TAG, "Packet type: %d", ws_pkt.type);

    ret = identifyPacket(ws_pkt, req);  
    if (ret != ESP_OK) {
        ESP_LOGE(WS_TAG, "Packet identification failed with %d", ret);
    }
    free(buf);

    // ret = httpd_ws_send_frame(req, &ws_pkt);
    // if (ret != ESP_OK) {
    //     ESP_LOGE(WS_TAG, "httpd_ws_send_frame failed with %d", ret);
    // }
    // free(buf);
    return ret;
}