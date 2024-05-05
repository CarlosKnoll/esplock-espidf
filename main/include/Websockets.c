#include <h_Webserver.h>
#include <h_SPIFFS.h>
#include <h_RFID.h>
#include <h_Websockets.h>

static char *WS_TAG = "ws";
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

    ESP_LOGW("DEBUG","Current payload: (%s)", ws_pkt.payload);

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    if (memFree==true){
        free(data);
        memFree=false;
    }
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
    operation = 0;
    return ret;
}


static esp_err_t identifyPacket(httpd_ws_frame_t ws_pkt, httpd_req_t *req){
    //Check for message from users webpage
    //Current state: response is an identifier unique to each possible websocket message received
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strstr((char*)ws_pkt.payload,"populateUsers") != NULL) {
        ESP_LOGW("TODO", "Temporary hardcoded response for populateUsers.");
        ESP_LOGW("TODO", "Check for page number availability.");
        data = "users#oldestID=1;data=12,Carlos,BD5777F0;11,Cartão,C489122B"; 
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strstr((char*)ws_pkt.payload,"removeUser") != NULL) {
        ESP_LOGW("TODO", "Temporary hardcoded response for removeUser.");
        data = "updateUsers#add";
        return GENERIC_HANDLER(req->handle, req);
    }

    //Check for message from add users webpage
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strstr((char*)ws_pkt.payload,"addUser") != NULL) {
        ESP_LOGW("TODO", "Temporary hardcoded response for addUser.");
        data = "success#add";
        //data = "unavailable";
        return GENERIC_HANDLER(req->handle, req);
    }
    
    //Check for message from access webpage    
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strstr((char*)ws_pkt.payload,"populateAccess") != NULL) {
        ESP_LOGW("TODO", "Temporary hardcoded response for populateAccess.");
        ESP_LOGW("TODO", "Check for page number availability.");
        data = "access#oldestID=1;data=11,Cartão,C489122B,05/05/24 09:52:05,Entrada;10,Carlos,BD5777F0,05/05/24 09:51:39,Saída;9,Carlos,BD5777F0,26/03/24 18:17:00,Entrada";
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strcmp((char*)ws_pkt.payload,"clear") == 0) {
        ESP_LOGW("TODO", "Temporary response for clear.");
        data = "Acessos: Limpar DB";
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strcmp((char*)ws_pkt.payload,"get") == 0) {
        ESP_LOGW("TODO", "Temporary response for get.");
        data = "Acessos: Download DB";
        return GENERIC_HANDLER(req->handle, req);
    }

    //Check for message from RFID readings
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && (strcmp((char*)ws_pkt.payload,"loopRFID") == 0)) { //If message received == "readRFID", then...
        tag_INT = 0;
        return ESP_OK;
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && (strcmp((char*)ws_pkt.payload,"readRFID") == 0)) { //If message received == "readRFID", then...
        operation = 1;      //Which RFID operation is being requested?
        memFree = true;     //Will we need to free data memory later?

        char * tempdata = "newUserId=";
        uint64_t newtag = tag_INT;

        char buff[12]; //How to determine dynamically?
        sprintf(buff, "%" PRIX64, newtag);

        data = malloc(strlen(tempdata)+strlen(buff));
        strcpy(data,tempdata);
        strcat(data,buff);
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && (strcmp((char*)ws_pkt.payload,"accessRFID") == 0 )) { //If message received == "accessRFID", then...
        ESP_LOGW("TODO", "Temporary hardcoded username for new access.");
        operation = 2;      //Which RFID operation is being requested?
        memFree = true;     //Will we need to free data memory later?

        char * tempdata = "NewAccess=Username;";
        uint64_t newtag = tag_INT;

        char buff[12]; //How to determine dynamically?
        sprintf(buff, "%" PRIX64, newtag);

        data = malloc(strlen(tempdata)+strlen(buff));
        strcpy(data,tempdata);
        strcat(data,buff);
                
        return GENERIC_HANDLER(req->handle, req);
    }
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && (strcmp((char*)ws_pkt.payload,"cancelRFID") == 0)) { //If message received == "cancelRFID, then...
        operation = 0;
        data = "cancel"; // Cancel specific RFID read request
        return GENERIC_HANDLER(req->handle, req);
    }

    //Check for message to update time
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strstr((char*)ws_pkt.payload,"epoch") != NULL) {
        ESP_LOGW("TODO", "Update internal clock.");
        return ESP_OK; //No return needed for time ws message
    }

    else{
        return ESP_FAIL;
    }
}

esp_err_t ws_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET) {
        ESP_LOGI(WS_TAG, "Handshake done");
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