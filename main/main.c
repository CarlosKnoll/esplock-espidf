#include "include/AccessPoint.c"
#include "include/SPIFFS.c"
#include "include/Webserver.c"
#include "include/Websockets.c"
#include "include/RFID.c"
#include "include/DataMgmt.c"


void app_main(void)
{ 
    nvs_init();
    softap_init();

    spiffs_init();
    rc522_init();

    /* Start the server for the first time */
    webserver_init();
    

}