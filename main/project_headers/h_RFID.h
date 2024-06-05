#ifndef RFID_h
#define RFID_h

#include <esp_log.h>
#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "rc522.h"

bool memFree = false;
int operation = 0; //0 = Only print; 1 = Print and process access; 2 = Print and returns tag to new user page; 3 = Print and returns tag to new access page
uint64_t tag_INT;

void check_tag();

#endif