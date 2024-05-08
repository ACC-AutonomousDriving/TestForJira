/**********************************************************************************************************************
 * \file GTM_TOM_PWM.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "GTM_TOM_PWM.h"
#include "Ifx_Types.h"
#include "IfxGtm_Tom_Pwm.h"
#include "IfxStm.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_TOM    20                                      /* Interrupt priority number                    */
#define LED                 IfxGtm_TOM0_2_TOUT104_P10_2_OUT         /* LED which will be driven by the PWM          */
#define DC_MOTOR            IfxGtm_TOM0_3_TOUT105_P10_3_OUT
#define PWM_PERIOD          50000                                   /* PWM period for the TOM                       */
#define FADE_STEP           PWM_PERIOD / 100                        /* PWM duty cycle for the TOM                   */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxGtm_Tom_Pwm_Config g_tomConfig;                                  /* Timer configuration structure                */
IfxGtm_Tom_Pwm_Driver g_tomDriver;                                  /* Timer Driver structure                       */
uint32 g_fadeValue = 0;                                             /* Fade value, starting from 0                  */
sint8 g_fadeDir = 1;                                                /* Fade direction variable                      */

/*********************************************************************************************************************/
/*-----------------------------------------------Private variables-------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void setDutyCycle(uint16 dutyCycle);                                /* Function to set the duty cycle of the PWM    */

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* This function initializes the TOM */
void initGtmTomPwm(void)
{
    IfxGtm_enable(&MODULE_GTM);                                     /* Enable GTM                                   */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);   /* Enable the FXU clock                         */

    /* Initialize the configuration structure with default parameters */
    IfxGtm_Tom_Pwm_initConfig(&g_tomConfig, &MODULE_GTM);

    g_tomConfig.tom = DC_MOTOR.tom;                                      /* Select the TOM depending on the LED          */
    g_tomConfig.tomChannel = DC_MOTOR.channel;                           /* Select the channel depending on the LED      */
    g_tomConfig.period = PWM_PERIOD;                                /* Set the timer period                         */
    g_tomConfig.pin.outputPin = &DC_MOTOR;                               /* Set the LED port pin as output               */
    g_tomConfig.synchronousUpdateEnabled = TRUE;                    /* Enable synchronous update                    */

    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);                /* Initialize the GTM TOM                       */
    IfxGtm_Tom_Pwm_start(&g_tomDriver, TRUE);                       /* Start the PWM                                */
}

/* This function creates the fade effect for the LED */
void fadeLED(void)
{
    if((g_fadeValue + FADE_STEP) >= PWM_PERIOD)
    {
        g_fadeDir = -1;                                             /* Set the direction of the fade                */
    }
    else if((g_fadeValue - FADE_STEP) <= 0)
    {
        g_fadeDir = 1;                                              /* Set the direction of the fade                */
    }
    g_fadeValue += g_fadeDir * FADE_STEP;                           /* Calculation of the new duty cycle            */
    setDutyCycle(g_fadeValue);                                      /* Set the duty cycle of the PWM                */
}

/* This function sets the duty cycle of the PWM */
void setDutyCycle(uint16 dutyCycle)
{
    Ifx_TickTime ticksFor10us = IfxStm_getTicksFromMicroseconds(BSP_DEFAULT_TIMER, 10);
    g_tomConfig.dutyCycle = dutyCycle;                              /* Change the value of the duty cycle           */
    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);                /* Re-initialize the PWM                        */
    waitTime(ticksFor10us * 50);
}
