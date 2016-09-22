#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mqtt.h"
#include "MQTTClient.h"
#include <FreeRTOS.h>
#include <task.h>
#include "processcheck_task.h"

#include "credentials.h"

#include "logging.h"

static MqttErrorCode MqttConnect(void);
static size_t SerializeData(NameValue* data, char* msg);

static Network mqttNet;
static Client mqttClient;

#define MQTT_BUFF_SIZE 1000
static uint8_t buf[MQTT_BUFF_SIZE];
static uint8_t readbuf[MQTT_BUFF_SIZE];

static uint8_t MESSAGE_ID = 0;
static bool deletePolling = false;
static uint32_t pollingPeriod = 1000;
static int32_t MQTT_YIELD_TIMEOUT = 50;

int MqttInit(void)
{
	DEBUG_PRINT("Mqtt Init");

    MqttErrorCode returnValue = FAILURE;
    NewNetwork(&mqttNet);
    returnValue = MqttConnect();
    if(SUCCESS == returnValue)
    {
        DEBUG_PRINT("Mqtt Success");
        deletePolling = false;
    }
    else
    {
        ERR_PRINT("Mqtt initialization Failed");
    }

    return returnValue;
}

int MqttReconnect(void)
{
    return MqttConnect();
}

void MqttDeinit(void)
{
	DEBUG_PRINT("Mqtt disconnect!");
    mqttNet.disconnect(&mqttNet);
}

int MqttSendData(NameValue* data)
{
    MQTTMessage msg;
    msg.id = MESSAGE_ID++;
    msg.qos = 0;
    char msgBuff[MQTT_BUFF_SIZE];
    msg.payload = msgBuff;
    msg.payloadlen = SerializeData(data, msgBuff);
    int ret = MQTTPublish(&mqttClient, MQTT_TOPIC"data", &msg);

    return ret;
}

int MqttSubscribe(messageHandler callback)
{
    int ret = MQTTSubscribe(&mqttClient, MQTT_TOPIC "cmd", QOS0, callback);

    DEBUG_PRINT("MqttSubscribe:: %s %d", MQTT_TOPIC "cmd", ret);

    return ret;
}

void MqttYield(void* context)
{
	context = context;
    for (;;)
    {
        ProcessCheck_ControlFlag(xTaskGetCurrentTaskHandle());
        MQTTYield(&mqttClient, MQTT_YIELD_TIMEOUT);
        vTaskDelay(pollingPeriod);
    }
}

MqttErrorCode MqttConnect(void)
{
    MqttErrorCode ret = FAILURE;
    ret = ConnectNetwork(&mqttNet, (int8_t*)MQTT_SERVER, MQTT_SERVER_PORT);
    if(SUCCESS == ret)
    {
        MQTTClient(&mqttClient, &mqttNet, 1000, buf, MQTT_BUFF_SIZE, readbuf, MQTT_BUFF_SIZE);

        MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
        data.willFlag = 0;
        data.MQTTVersion = 3;
        data.clientID.cstring = MQTT_CLIENTID;
        data.username.cstring = MQTT_USER;
        data.password.cstring = MQTT_PASSWORD;

        data.keepAliveInterval = 10;
        data.cleansession = 1;
        DEBUG_PRINT("Connecting to %s %d with user: %s, ps: %s",
                MQTT_SERVER,
                MQTT_SERVER_PORT,
                MQTT_USER,
                MQTT_PASSWORD);

        ret = MQTTConnect(&mqttClient, &data);
        if(SUCCESS == ret)
        {
        	DEBUG_PRINT("Connected %d", ret);
        }
        else
        {
        	ERR_PRINT("Mqtt Connection failed!");
        }
    } else {
    	ERR_PRINT("Connect network failed - %d", ret);
    }
    return ret;
}

static size_t SerializeData(NameValue* data, char* msg)
{
    return sprintf(msg,
                   "{\"meaning\":\"%s\",\"path\":\"\",\"value\":%s}",
                   data->name,
                   data->value);
}

void MqttStopPolling(void)
{
    deletePolling = true;
}

void MqttSetPollingPeriod(uint32_t period)
{
    pollingPeriod = period;
}

