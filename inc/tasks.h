#pragma once

#include <stdio.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

extern xTaskHandle tickTaskHandle;

void TickInit(void);
void TickKill(void);
void CommandHandlerInit(void);
void WifiConnectInit(void);
void MqttConnectInit(void);
void Restart(void);
