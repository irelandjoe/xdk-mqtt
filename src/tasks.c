#include "tasks.h"
#include "em_wdog.h"
#include "sensordata.h"
#include "leds.h"
#include "mqtt.h"
#include "wifi.h"
#include "controls.h"
#include <FreeRTOS.h>
#include <projdefs.h>
#include <semphr.h>
#include <timers.h>
#include <queue.h>
#include "processcheck_task.h"
#include "logging.h"
#include "xdkwdog.h"

#define PUBLISH_PERIOD  3000;
#define MQTT_CONN_MAX_FAILS	5


static uint8_t Tick(void);
static void Tasks_stateMachine(void *context);
static void Tasks_Uptime(void *params);
static void Tasks_CommandHandlerInit(void);

static xTaskHandle mqttYieldTaskHandle;
static xTaskHandle mainTaskHandle;

static xTimerHandle sysTickIntHandle = NULL;
static SemaphoreHandle_t xSemaphore;


static uint8_t restart = 0;
static uint32_t uptime = 0;

static const uint32_t RECONNECT_PERIOD = 4000;
static const uint32_t TASK_STACK_SIZE = 8000;
static const uint32_t TASK_PRIO = 4;

uint32_t tickPeriod = PUBLISH_PERIOD;

XDK_Retcode_E Tasks_init(void)
{
    xSemaphore = xSemaphoreCreateMutex();

    TASK_CREATE_WITHCHECK(Tasks_stateMachine, (const char *) "State machine", TASK_STACK_SIZE, TASK_PRIO, mainTaskHandle);

    sysTickIntHandle = xTimerCreate((const char *) "uptime",
                                    ( 1000 / portTICK_PERIOD_MS ),
                                    pdTRUE,
                                    0,
                                    Tasks_Uptime);

    if (NULL == sysTickIntHandle)
    {
        FATAL_PRINT("Not enough memory to create uptime Timer!");
        return XDK_RETCODE_FAILURE;
    }
    else if(pdFAIL == xTimerStart(sysTickIntHandle, 0xFFFF))
    {
        FATAL_PRINT("Not enough memory to start uptime Timer!");
        return XDK_RETCODE_FAILURE;
    }

    if (xSemaphore == NULL)
    {
        FATAL_PRINT("Error occurred in creating mutex");
        return XDK_RETCODE_FAILURE;
    }

    return XDK_RETCODE_SUCCESS;

}

void Tasks_restart(void)
{
    if (xSemaphoreTake(xSemaphore, ( TickType_t ) 10) == pdTRUE)
    {
        restart = 1;
        xSemaphoreGive(xSemaphore);
    }
    else
    {
        WARN_PRINT("Cannot take the semaphore!");
    }
}


/**
 * Uptime counter task
 */
static void Tasks_Uptime(void *params)
{
    uptime++;
    if ((uptime % 60) == 0)
    {
        DEBUG_PRINT("uptime: %lu (seconds)", uptime);
    }
}

/*
 * FreeRTOS task
 * State machine :
 *   1) WiFi connect -> 2
 *   2) MQTT connect -> 3
 *   3) Init MQTT subscribe task -> 4
 *   4) Collecting date from sensors -> 4
 *
 * If any problem go to RESTART state - restart by watchdog
 */
static void Tasks_stateMachine(void *context)
{
    TASK_STATES state = WIFI_INIT;
    context = context;

    uint8_t fail_count = 0;
    while (1)
    {

        ProcessCheck_ControlFlag(xTaskGetCurrentTaskHandle());

        if (xSemaphoreTake(xSemaphore, ( TickType_t ) 10) == pdTRUE)
        {
            if (restart)
            {
                state = RESTART;
                restart = 0;
            }
            xSemaphoreGive(xSemaphore);
        }

        switch (state) {
        case WIFI_INIT:
            if (WiFiInit() == XDK_RETCODE_SUCCESS)
            {
                state = MQTT_CONN;
            }
            else
            {
                vTaskDelay(RECONNECT_PERIOD / portTICK_RATE_MS);
            }
            break;
        case MQTT_CONN:
            if (MqttInit() == XDK_RETCODE_SUCCESS)
            {
                state = MQTT_SUB;
            } 
            else 
            {
            	if (fail_count++ > MQTT_CONN_MAX_FAILS)
            	{
                	state = RESTART;
                	fail_count = 0;
            	}
            	else
            	{
                	vTaskDelay(RECONNECT_PERIOD / portTICK_RATE_MS);
            	}
            }
            break;
        case MQTT_SUB:
            Tasks_CommandHandlerInit();
            state = RUNNING_STATE;
            break;
        case RUNNING_STATE:
            if (Tick() == XDK_RETCODE_FAILURE)
            {
                state = RESTART;
            }
            else
            {
                vTaskDelay(tickPeriod / portTICK_RATE_MS);
            }
            break;
        case ERROR_STATE:
            //watchdog restart
            //not in used
            break;
        case RESTART:
            DEBUG_PRINT("Restarting by WDG");
            XdkWdog_RestartByWdog();
            vTaskDelay(3000 / portTICK_RATE_MS);
            break;
        }
    }
}

/**
 * Collecting data from activated sensors and sending to cloud
 */
static XDK_Retcode_E Tick(void)
{
    SensorData data;

    for(uint32_t sensor = 0; sensor < NUM_SENSORS; ++sensor)
    {
        if(enabledSensors[sensor])
        {
            // Get data
            sensors[sensor](&data);

            for(uint32_t meas = 0; meas < data.numMeas; ++meas)
            {
                if(0 > MqttSendData(&data.meas[meas]))
                {
                    DEBUG_PRINT("Sending data FAILED! Restarting WiFi and MQTT!");
                    return XDK_RETCODE_FAILURE;
                }
                else
                {
                    LedSet(ORANGE_LED, LED_SET_TOGGLE);
                }
            }
        }
    }
    return XDK_RETCODE_SUCCESS;
}

/*
 * Set the period of data collecting
 * period - publish period in ms
 */
void Tasks_PublishPeriod(uint32_t period)
{
    if (period == 0)
    {
        period = PUBLISH_PERIOD;
    }

    tickPeriod = period;
}

/**
 * Initialization of subscribe task
 */
static void Tasks_CommandHandlerInit(void)
{
    MqttSubscribe(&CommandConfigHandler);
    TASK_CREATE_WITHCHECK(MqttYield, (const char *) "MQTT Commands", 1000, TASK_PRIO - 1, mqttYieldTaskHandle);
}
