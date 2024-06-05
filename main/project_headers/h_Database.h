#ifndef Database_h
#define Database_h

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dirent.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "ff.h"
#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sqlite3.h"
#include "sdmmc_cmd.h"


#define MAX_FILE_NAME_LEN 100
#define MAX_STR_LEN 500

void dbCheck(uint64_t tag);

#endif