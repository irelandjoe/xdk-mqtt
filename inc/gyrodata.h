#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

uint8_t GyroInit(void);
void GyroDeinit(void);
void GyroGetData(SensorData* data);
