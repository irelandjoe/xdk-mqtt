#include <stdio.h>
#include "sensordata.h"
#include "logging.h"

uint8_t SensorInit(SensorInitializer init, void* handle, const char* label)
{
    if(SENSOR_SUCCESS == init(handle))
    {
        DEBUG_PRINT("%s initialization Success", label);
        return 1;
    }

    ERR_PRINT("%s initialization FAILED", label);

    return 0;
}

void SensorDataClear(SensorData* data)
{
    data->numMeas = 0;
    for(int i = 0; i < MAX_MEASUREMENTS; ++i)
    {
        data->meas[i].name[0]  = '\0';
        data->meas[i].value[0] = '\0';
    }
}
