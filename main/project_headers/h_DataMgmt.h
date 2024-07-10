#ifndef DataMgmt_h
#define DataMgmt_h

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h" 
#include "esp_spiffs.h"
#include <esp_http_server.h>

char dataSys[1024];
char dataSys_final[1024];

char* getData(char* PATH);
char *getUser(uint64_t TAG);

#endif