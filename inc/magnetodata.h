#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

uint8_t MagnetoInit(void);
void MagnetoDeinit(void);
void MagnetoGetData(SensorData* data);
