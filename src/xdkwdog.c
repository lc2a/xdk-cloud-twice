/*
 * xdkwdog.c
 *
 *  Created on: Aug 12, 2016
 *      Author: krcka
 */
#include <stdio.h>
#include <stdint.h>

#include "logging.h"
#include "em_wdog.h"


void XdkWDog_Init(void)
{
    WDOG_Init_TypeDef wdConfig = { false,              /* Do not Start watchdog when init done */
                                   false,              /* WDOG not counting during debug halt */
                                   false,              /* WDOG not counting when in EM2 */
                                   false,              /* WDOG not counting when in EM3 */
                                   false,              /* EM4 can be entered */
                                   false,              /* Do not block disabling LFRCO/LFXO in CMU */
                                   false,              /* Do not lock WDOG configuration (if locked, reset needed to unlock) */
                                   wdogClkSelLFXO,     /* Select 32768HZ LFXO as clock source for WDOG oscillator */
                                   wdogPeriod_256k     /* Set longest possible timeout period */
                                };
    DEBUG_PRINT("Initializing Watch Dog");
    WDOG_Init(&wdConfig);
    DEBUG_PRINT("Turning on Watch Dog\n");
    WDOG_Enable(true);
    DEBUG_PRINT("Watch Dog ON\n");
}

void XdkWdog_Feed(void)
{
    WDOG_Feed();
}

void XdkWdog_RestartByWdog(void)
{
    WDOG_Init_TypeDef wdConfig = { false,              /* Do not Start watchdog when init done */
                                   false,              /* WDOG not counting during debug halt */
                                   false,              /* WDOG not counting when in EM2 */
                                   false,              /* WDOG not counting when in EM3 */
                                   false,              /* EM4 can be entered */
                                   false,              /* Do not block disabling LFRCO/LFXO in CMU */
                                   false,              /* Do not lock WDOG configuration (if locked, reset needed to unlock) */
                                   wdogClkSelLFXO,     /* Select 32768HZ LFXO as clock source for WDOG oscillator */
                                   wdogPeriod_9     /* Set longest possible timeout period */
                                };
    WDOG_Enable(false);
    WDOG_Init(&wdConfig);
    WDOG_Enable(true);
}
