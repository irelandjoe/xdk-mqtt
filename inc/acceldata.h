#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

uint8_t AccelInit(void);
void AccelDeinit(void);
void AccelGetData(SensorData* data);
