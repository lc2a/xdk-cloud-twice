#include "BCDS_Magnetometer.h"
#include "XdkSensorHandle.h"
#include "magnetodata.h"

#include "retcode.h"
#include "logging.h"

static const char MAGNETO_LABEL[] = "BMM150 Magnetometer";

static void FillMagnetoData(SensorData* data, Magnetometer_XyzData_T* meas, int cloud_ver)
{
	switch (CLOUD_VER){
			case 1:
				data->numMeas = 1;
				snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "magneticField");

				snprintf(data->meas[0].value,
						SENSOR_VALUE_SIZE,
						"{\"x\":%f, \"y\":%f, \"z\":%f}",
						(float)meas->xAxisData,
						(float)meas->yAxisData,
						(float)meas->zAxisData);
				break;
			case 2:
				data->numMeas = 3;
				snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "magneticField_x");
				snprintf(data->meas[1].name, SENSOR_NAME_SIZE, "%s", "magneticField_y");
				snprintf(data->meas[2].name, SENSOR_NAME_SIZE, "%s", "magneticField_z");

				snprintf(data->meas[0].value, SENSOR_VALUE_SIZE, "%f",(float)meas->xAxisData);
				snprintf(data->meas[1].value, SENSOR_VALUE_SIZE, "%f",(float)meas->yAxisData);
				snprintf(data->meas[2].value, SENSOR_VALUE_SIZE, "%f",(float)meas->zAxisData);
				break;
		}
}

Retcode_T PrivateInit(void* handle)
{
    return Magnetometer_init((Magnetometer_HandlePtr_T)handle);
}

XDK_Retcode_E MagnetoInit(void)
{
    return SensorInit(&PrivateInit,
               xdkMagnetometer_BMM150_Handle,
               MAGNETO_LABEL);
}

void MagnetoDeinit(void)
{
    Magnetometer_deInit(xdkMagnetometer_BMM150_Handle);
}

void MagnetoGetData(SensorData* data, int cloud_ver)
{
    Retcode_T returnValue = SENSOR_ERROR;
    Magnetometer_XyzData_T getMagData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = Magnetometer_readXyzLsbData(xdkMagnetometer_BMM150_Handle, &getMagData);
    if(SENSOR_SUCCESS == returnValue)
    {
    	TRACE_PRINT("%s Raw Data : x = %ld, y = %ld, z = %ld",
               MAGNETO_LABEL,
               (long int)getMagData.xAxisData,
               (long int)getMagData.yAxisData,
               (long int)getMagData.zAxisData);
    }
    else
    {
        WARN_PRINT("%s Raw Data read FAILED", MAGNETO_LABEL);
    }

    returnValue = Magnetometer_readXyzTeslaData(xdkMagnetometer_BMM150_Handle, &getMagData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillMagnetoData(data, &getMagData, cloud_ver);
        TRACE_PRINT("%s Converted Data : x = %ld uT, y = %ld uT, z = %ld uT",
               MAGNETO_LABEL,
               (long int)getMagData.xAxisData,
               (long int)getMagData.yAxisData,
               (long int)getMagData.zAxisData);
    }
    else
    {
    	WARN_PRINT("%s Converted Data read FAILED", MAGNETO_LABEL);
    }
}
