#include <stdio.h>
#include "sensordata.h"
#include "logging.h"
#include "retcode.h"

XDK_Retcode_E SensorInit(SensorInitializer init, void* handle, const char* label)
{
    if(SENSOR_SUCCESS == init(handle))
    {
        DEBUG_PRINT("%s initialization Success", label);
        return XDK_RETCODE_SUCCESS;
    }

    ERR_PRINT("%s initialization FAILED", label);

    return XDK_RETCODE_FAILURE;
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
