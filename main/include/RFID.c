#include <h_RFID.h>
#include <h_Database.h>

static const char* TAG = "RC522a";
static rc522_handle_t scanner;


static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;
    
    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
            rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
            ESP_LOGI(TAG, "Tag scanned (sn: %" PRIX64 ")", tag->serial_number);
            tag_INT = tag->serial_number;
            if(operation != 1){
                ESP_LOGW("TODO", "Log this new access into DB");
                
                uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_8BIT);
                ESP_LOGW("DEBUG", "RFID.c, after new reading event: Free bytes: %lu", freeHeapBytes);
            }
        }
    }
}

void rc522_init()
{
    rc522_config_t config = {
        .spi.host = VSPI_HOST,
        .spi.miso_gpio = 15, //19 Não funciona, diferente da versão com arduino
        .spi.mosi_gpio = 23,
        .spi.sck_gpio = 18,
        .spi.sda_gpio = 5,
    };


    rc522_create(&config, &scanner);
    rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
    rc522_start(scanner);
    xTaskCreate(check_tag, "Check for tag read", 16384, NULL, 4, NULL);
}

void check_tag(){
    while(1){
        if (tag_INT != 0){
            dbCheck(tag_INT);
            //vTaskDelete(NULL);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}