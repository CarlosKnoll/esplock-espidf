#include "include/AccessPoint.c"
#include "include/SPIFFS.c"
#include "include/Webserver.c"
#include "include/Websockets.c"
#include "include/RFID.c"
#include "include/DataMgmt.c"
#include "include/DeepSleep.c"
//#include "include/OLEDSetup.c"

#include "driver/gpio.h"

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

    // LED ON - Feedback sleep?
    // GPIO 25
    gpio_set_direction(25, GPIO_MODE_OUTPUT);
    gpio_set_level(25,1);


}