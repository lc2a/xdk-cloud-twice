#include "xdkbutton.h"
#include "BCDS_BSP_Button.h"
#include "FreeRTOS.h"
#include "BSP_BoardType.h"
#include "BCDS_BSP_LED.h"

#include "retcode.h"
#include "logging.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"

/* Routine to Initialize the Button module  */

static void processButton1Data(void * param1, uint32_t buttonstatus)
{
    BCDS_UNUSED(param1);
    Retcode_T retVal = RETCODE_OK;
    switch (buttonstatus)
    {
    case BSP_XDK_BUTTON_PRESS:
        {
        retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_TOGGLE);
        if (buttonstate[0]) {
        	buttonstate[0] = FALSE ;
        }
        else {
        	buttonstate[0] = TRUE ;
        }
        if (RETCODE_OK == retVal)
        {
            printf("PB1 Pressed \n\r");
        }
        else
        {
            printf("PB1 Pressed but setting LED state failed\n\r");
        }
    }
        break;

    case BSP_XDK_BUTTON_RELEASE:
    {
        if (RETCODE_OK == retVal)
        {
            printf("PB1 Released\n\r");
        }
        else
        {
            printf("PB1 Released but setting LED state failed\n\r");
        }
    }
        break;

    default:
        printf("FATAL Error:Unsolicited button event occurred for PB1 \n\r");
        break;

    }

}

static void processButton2Data(void * param1, uint32_t buttonstatus)
{
    BCDS_UNUSED(param1);
    Retcode_T retVal = RETCODE_OK;
    switch (buttonstatus)
    {
    case BSP_XDK_BUTTON_PRESS:
        {
        retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_TOGGLE);
        if (buttonstate[1]) {
        	buttonstate[1] = FALSE ;
        }
        else {
        	buttonstate[1] = TRUE ;
        }
        if (RETCODE_OK == retVal)
        {
            printf("PB1 Pressed \n\r");
        }
        else
        {
            printf("PB1 Pressed but setting LED state failed\n\r");
        }
    }
        break;

    case BSP_XDK_BUTTON_RELEASE:
    {
        if (RETCODE_OK == retVal)
        {
            printf("PB1 Released\n\r");
        }
        else
        {
            printf("PB1 Released but setting LED state failed\n\r");
        }
    }
        break;

    default:
        printf("FATAL Error:Unsolicited button event occurred for PB1 \n\r");
        break;

    }

}

void Button1Callback(uint32_t data)
{
    processButton1Data(NULL, data);
}

void Button2Callback(uint32_t data)
{
	processButton2Data(NULL, data);
}

Retcode_T ButtonInitialize(void)
{
    Retcode_T returnVal = RETCODE_OK;
    buttonstate[0] = FALSE;
    buttonstate[1] = FALSE;
    returnVal = BSP_Button_Connect();
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_Button_Enable((uint32_t) BSP_XDK_BUTTON_1, Button1Callback);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_Button_Enable((uint32_t) BSP_XDK_BUTTON_2, Button2Callback);
    }
    if (RETCODE_OK == returnVal)
    {
        printf("BUTTON Initialization success");
    }
    else
    {
        printf(" Error occurred in BUTTON Initialization routine %u \n", (unsigned int) returnVal);
    }

    return returnVal;
}

static void FillButtonData(SensorData* data, bool meas[2])
{
    data->numMeas = 2;
    snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "Button_1_state");
    snprintf(data->meas[1].name, SENSOR_NAME_SIZE, "%s", "Button_2_state");

    if (meas[0]){
    	snprintf(data->meas[0].value,
    			SENSOR_VALUE_SIZE,
				"%s",
				("true"));
    }
    else {
    	snprintf(data->meas[0].value,
    	    	SENSOR_VALUE_SIZE,
    			"%s",
    			("false"));
    }
    if (meas[1]){
        snprintf(data->meas[1].value,
        		SENSOR_VALUE_SIZE,
    			"%s",
    			("true"));
    }
    else {
        snprintf(data->meas[1].value,
        	    SENSOR_VALUE_SIZE,
        		"%s",
        		("false"));
    }
}

void ButtonGetData(SensorData* data)
{

   // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    FillButtonData(data,buttonstate);
}
