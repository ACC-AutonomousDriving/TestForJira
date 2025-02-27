/**********************************************************************************************************************
 * \file Appscheduling.c
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
#include "AppScheduling.h"
#include "Driver_Stm.h"

#include "GTM_TOM_PWM.h"
#include <math.h>


/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define CH_A        &MODULE_P14,0
#define CH_B        &MODULE_P14,1
#define LOW_VAL     0
#define HIGH_VAL    1

#define PI 3.141592
#define TICKS_PER_REVOLUTION 48
/*********************************************************************************************************************/
/*------------------------------------------------------Typedef-------------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
        uint32 u32nuCnt1ms;
        uint32 u32nuCnt10ms;
        uint32 u32nuCnt100ms;
        uint32 u32nuCnt1000ms;
        uint32 u32nuCnt5000ms;
}TestCnt;
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
int Encoder_Ticks = 0;

double radiansPerTick = 0.0;  // 한 틱당 라디안
double totalRadians = 0.0;    // 총 라디안
double speedRadPerSec = 0.0;     // 초당 라디안
double Rad_per_Sec = 0.0f;

/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/
TestCnt stTestCnt;
uint8 _tmp = 0;
uint8 prev_CHA = 0;
uint8 prev_CHB = 0;
uint32 prev_Encoder_Ticks = 0;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
static void AppTask1ms(void);
static void AppTask10ms(void);
static void AppTask100ms(void);
static void AppTask1000ms(void);
static void AppTask5000ms(void);

void AppNoTask(void);
double calculateSpeed_Rads(int encoderTicks, double deltaTime);

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

static void AppTask1ms(void){
    stTestCnt.u32nuCnt1ms++;
}
static void AppTask10ms(void){
    stTestCnt.u32nuCnt10ms++;
}
static void AppTask100ms(void){
    stTestCnt.u32nuCnt100ms++;
//    IfxPort_togglePin(&MODULE_P10,1);
}
static void AppTask1000ms(void){
    stTestCnt.u32nuCnt1000ms++;
//    IfxPort_togglePin(&MODULE_P10,2);
    Rad_per_Sec = calculateSpeed_Rads(Encoder_Ticks, 1.0); // rad/s 구하기
    Encoder_Ticks = 0; // 초기화.
}
static void AppTask5000ms(void){
    stTestCnt.u32nuCnt5000ms++;
    if(_tmp ^= 1) setDutyCycle(50000); // 4294967295 (최대)
    else setDutyCycle(20000);

}

void AppScheduling(void){
    AppNoTask();

    if(stSchedulingInfo.u8nuScheduling1msFlag == 1u){
        stSchedulingInfo.u8nuScheduling1msFlag = 0u;
        AppTask1ms();

        if(stSchedulingInfo.u8nuScheduling10msFlag == 1u){
            stSchedulingInfo.u8nuScheduling10msFlag = 0u;
            AppTask10ms();
        }

        if(stSchedulingInfo.u8nuScheduling100msFlag == 1u){
            stSchedulingInfo.u8nuScheduling100msFlag = 0u;
            AppTask100ms();
        }

        if(stSchedulingInfo.u8nuScheduling1000msFlag == 1u){
            stSchedulingInfo.u8nuScheduling1000msFlag = 0u;
            AppTask1000ms();
        }

        if(stSchedulingInfo.u8nuScheduling5000msFlag == 1u){
            stSchedulingInfo.u8nuScheduling5000msFlag = 0u;
            AppTask5000ms();
        }
    }

}

void AppNoTask(){
    uint8 CH_A_cur_val = IfxPort_getPinState(CH_A);
    uint8 CH_B_cur_val = IfxPort_getPinState(CH_B);

    /*if(prev_CHA != CH_A_cur_val){
        Encoder_Ticks++;
    }
    else if(prev_CHB != CH_B_cur_val){
        Encoder_Ticks++;
    }*/

    // Encoder Tick counting
    if(prev_CHA != CH_A_cur_val){
        if(CH_A_cur_val == HIGH_VAL){
            if(CH_B_cur_val == LOW_VAL){
                Encoder_Ticks++;
            }
            else if(CH_B_cur_val == HIGH_VAL){
                Encoder_Ticks--;
            }
        }
        else if(CH_A_cur_val == LOW_VAL){
            if(CH_B_cur_val == HIGH_VAL){
                Encoder_Ticks++;
            }
            else if(CH_B_cur_val == LOW_VAL){
                Encoder_Ticks--;
            }
        }
    }

    if(prev_CHB != CH_B_cur_val){
        if(CH_B_cur_val == HIGH_VAL){
            if(CH_A_cur_val == HIGH_VAL){
                Encoder_Ticks++;
            }
            else if(CH_A_cur_val == LOW_VAL){
                Encoder_Ticks--;
            }
        }
        else if(CH_B_cur_val == LOW_VAL){
            if(CH_A_cur_val == LOW_VAL){
                Encoder_Ticks++;
            }
            else if(CH_A_cur_val == HIGH_VAL){
                Encoder_Ticks--;
            }
        }
    }

    prev_CHA = CH_A_cur_val;
    prev_CHB = CH_B_cur_val;

    return;
}

void Encoder_Two_Channel_Init(){
    IfxPort_setPinModeInput(CH_A, IfxPort_InputMode_pullUp);
    IfxPort_setPinModeInput(CH_B, IfxPort_InputMode_pullUp);
}

double calculateSpeed_Rads(int encoderTicks, double deltaTime) {
    double radiansPerTick = 2 * PI / TICKS_PER_REVOLUTION;  // 한 틱당 라디안
    double totalRadians = encoderTicks * radiansPerTick;    // 총 라디안
    double speedRadPerSec = totalRadians / deltaTime;       // 초당 라디안

    return speedRadPerSec;
}
