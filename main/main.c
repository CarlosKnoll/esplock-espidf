/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <softAP.c>
#include <webServer.c>
#include <spiffs.c>

void app_main(void)
{
    static httpd_handle_t server = NULL;
  
    start_nvs();
    wifi_init_softap();

    spiffs_init();

    /* Start the server for the first time */
    server = start_webserver();
}