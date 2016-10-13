#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "MQTTClient.h"
#include "retcode.h"

typedef enum returnCode MqttErrorCode;

XDK_Retcode_E  MqttInit(void);
void MqttDeinit(void);
XDK_Retcode_E  MqttReconnect(void);
int  MqttSendData(NameValue* data);
int  MqttSubscribe(messageHandler callback);
void MqttYield(void* context);
void MqttStopPolling(void);
void MqttSetPollingPeriod(uint32_t period);
