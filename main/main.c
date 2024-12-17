#include "include/AccessPoint.c"
#include "include/SPIFFS.c"
#include "include/Webserver.c"
#include "include/Websockets.c"
#include "include/RFID.c"
<<<<<<< Updated upstream
=======
#include "include/DataMgmt.c"
#include "include/DeepSleep.c"
//#include "include/OLEDSetup.c"
>>>>>>> Stashed changes


void app_main(void)
{ 
    nvs_init();
    softap_init();

    spiffs_init();
    rc522_init();
    deepSleep_init();
    //oled_setup();

    /* Start the server for the first time */
    webserver_init();
    

}