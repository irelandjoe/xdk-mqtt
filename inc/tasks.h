#pragma once

#include <stdio.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

uint8_t Tasks_init(void);
void Tasks_restart(void);
void Tasks_PublishPeriod(uint32_t period);

typedef enum task_states {
    WIFI_INIT = 0, //
    MQTT_CONN,
    MQTT_SUB,
    RUNNING_STATE,
    ERROR_STATE,
    RESTART
} TASK_STATES;
