#include "leds.h"
#include <stdio.h>
#include <stdint.h>
#include "BSP_BoardType.h"

/* Routine to Initialize the LED */
Retcode_T LedInit(void)
{
    Retcode_T returnVal = RETCODE_OK;
    returnVal = BSP_LED_Connect();
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) BSP_XDK_LED_R);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) BSP_XDK_LED_O);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) BSP_XDK_LED_Y);
    }
    if (RETCODE_OK == returnVal)
    {
        printf("LED Initialization succeed without error %u \n", (unsigned int) returnVal);
    }
    else
    {
        printf(" Error occurred in LED Initialization routine %u \n", (unsigned int) returnVal);
    }
    return returnVal;
}

Retcode_T LedDeinit(void)
{
    Retcode_T returnVal = RETCODE_OK;
    returnVal = BSP_LED_Connect();
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Disable((uint32_t) BSP_XDK_LED_R);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Disable((uint32_t) BSP_XDK_LED_O);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Disable((uint32_t) BSP_XDK_LED_Y);
    }
    if (RETCODE_OK == returnVal)
    {
        printf("LED Deinit succeed without error %u \n", (unsigned int) returnVal);
    }
    else
    {
        printf(" Error occurred in LED Deinit routine %u \n", (unsigned int) returnVal);
    }
    return returnVal;
}
void LedSet(uint32_t led, uint32_t op)
{
    if(led < (uint32_t)NUM_LEDS)
    {
    	BSP_LED_Switch(led, op);
    }
}
