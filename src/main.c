#include <stdio.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <timers.h>
#include "XdkSystemStartup.h"
#include "mqtt.h"

// sensors
#include "sensordata.h"
#include "envdata.h"
#include "acceldata.h"
#include "lightdata.h"
#include "gyrodata.h"
#include "magnetodata.h"

// peripherals
#include "leds.h"
#include "wifi.h"
#include "xdkbutton.h"

#include "tasks.h"
#include "processcheck_task.h"
#include "controls.h"
#include "xdkwdog.h"
#include "logging.h"

#include "URU_usbResetUtility_ih.h"


SensorGetter sensors[] = {&EnvSensorGetData,
                          &AccelGetData,
                          &LightGetData,
                          &GyroGetData,
                          &MagnetoGetData};

bool enabledSensors[] = {true, // Env Sensor
                         true, // Accel Sensor
                         true, // Light Sensor
                         true, // Gyro Sensor
                         true  // Magneto Sensor
                        };

void ButtonOneCallback(void *handle, uint32_t userParameter);


int main(void)
{
    systemStartup();
}

void appInitSystem(xTimerHandle xTimer)
{
    (void) (xTimer);
    ProcessCheck_Init();
    LedInit();
    ButtonOneInit(&ButtonOneCallback);
    MagnetoInit();
    GyroInit();
    LightInit();
    AccelInit();
    EnvSensorInit();
    XdkWDog_Init();
    Tasks_init();
}

void ButtonOneCallback(void *handle, uint32_t userParameter)
{
    TRACE_PRINT("Button %d pressed", (int)userParameter);

    if(IsButtonPressed(handle))
    {
    	TRACE_PRINT("Button pressed!");
        LedSet(RED_LED, LED_SET_ON);
    }

    if(IsButtonReleased(handle))
    {
    	TRACE_PRINT("Button released!");
        LedSet(RED_LED, LED_SET_OFF);
        Tasks_restart();
    }
}

