#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

uint8_t LightInit(void);
void LightDeinit(void);
void LightGetData(SensorData* data);
