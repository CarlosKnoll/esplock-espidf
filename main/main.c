#include <softAP.c>
#include <webServer.c>
#include <spiffs.c>
#include <websockets.c>

void app_main(void)
{
    static httpd_handle_t server = NULL;
  
    start_nvs();
    wifi_init_softap();

    spiffs_init();

    /* Start the server for the first time */
    server = start_webserver();
}