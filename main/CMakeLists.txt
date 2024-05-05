idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "project_headers" "include")

spiffs_create_partition_image(spiffs spiffs FLASH_IN_PROJECT)
