idf_component_register(SRCS "main.c" "softAP.c" "webServer.c" "spiffs.c"
                    INCLUDE_DIRS ".")

spiffs_create_partition_image(spiffs spiffs FLASH_IN_PROJECT)
