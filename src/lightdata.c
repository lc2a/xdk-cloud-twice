#include "BCDS_LightSensor.h"
#include "XdkSensorHandle.h"
#include "lightdata.h"

#include "retcode.h"
#include "logging.h"

static const char LIGHT_SENSOR_LABEL[] = "MAX09 Light Sensor";


static void FillLightData(SensorData* data, uint32_t meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "luminosity");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SIZE,
             "%f",
             (float)meas/1000.0);
}

Retcode_T LightPrivateInit(void* handle)
{
    return LightSensor_init((LightSensor_HandlePtr_T)handle);
}

XDK_Retcode_E LightInit(void)
{
    return SensorInit(&LightPrivateInit,
               xdkLightSensor_MAX44009_Handle,
               LIGHT_SENSOR_LABEL);
}

void LightDeinit(void)
{
    LightSensor_deInit(xdkLightSensor_MAX44009_Handle);
}

void LightGetData(SensorData* data)
{
    Retcode_T returnValue = SENSOR_ERROR;
    uint16_t luxRawData = 0;
    uint32_t milliLuxData = 0;

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = LightSensor_readRawData(xdkLightSensor_MAX44009_Handle, &luxRawData);
    if(SENSOR_SUCCESS == returnValue)
    {
        TRACE_PRINT("%s Raw Data : %x", LIGHT_SENSOR_LABEL, luxRawData);
    }
    else
    {
        WARN_PRINT("%s Raw Data read FAILED", LIGHT_SENSOR_LABEL);
    }

    returnValue = LightSensor_readLuxData(xdkLightSensor_MAX44009_Handle, &milliLuxData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillLightData(data, milliLuxData);
        TRACE_PRINT("%s Data : %d mlux", LIGHT_SENSOR_LABEL, (unsigned int)milliLuxData);
    }
    else
    {
    	WARN_PRINT("%s Data read FAILED", LIGHT_SENSOR_LABEL);
    }
}
