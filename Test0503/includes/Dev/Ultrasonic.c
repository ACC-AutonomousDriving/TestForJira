/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ultrasonic.h"

#include "Platform_Types.h"
#include "IfxPort.h"
#include "IfxPort.c"
#include "IfxCpu.h"
#include "IfxStm.h"

#include "Delay.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define TIMEOUT     23529

#define P00_4_IRQ      IfxScu_REQ7_P00_4_IN //Trigger
#define P00_4_PORT     &MODULE_P00
#define P00_4_PIN      4
#define P11_10_IRQ      IfxScu_REQ12_P11_10_IN //Echo
#define P11_10_PORT    &MODULE_P11
#define P11_10_PIN      10
/*********************************************************************************************************************/
/*---------------------------------------------global variables----------------------------------------------*/
/*********************************************************************************************************************/
uint32 estimation_start;
uint32 estimation_end;
uint32 estimations;
/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
//uint32 Pulse_In(IfxPort_Pin pin, uint8 state, uint32 timeout);


/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(ISR_Rising, 0, 0x10);
IFX_INTERRUPT(ISR_Falling, 1, 0x10);
void ISR_Rising(void){ //rising
        estimation_end = Get_Cur_Micros();
        estimations = (estimation_end - estimation_start);
}
//rising 일띠ㅐ 0부터 세고  falinig일때 저장, 단, TIMEOUT 됐을 시 falining 인터럽트에서 저장 0으로 해주기
void ISR_Falling(void){ //falling

    IfxPort_setPinLow(P00_4_PORT, P00_4_PIN);
    IfxPort_setPinLow(P00_4_PORT, P00_4_PIN);
    Delay_Us(2);
    IfxPort_setPinHigh(P00_4_PORT, P00_4_PIN);
    Delay_Us(10);
    IfxPort_setPinLow(P00_4_PORT, P00_4_PIN);
    estimation_start = Get_Cur_Micros();
}

void Init_Ultrasonic(void) {
    IfxPort_setPinModeOutput(P00_4_PORT, P00_4_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeInput(P11_10_PORT, P11_10_PIN, IfxPort_InputMode_pullUp);
}

float32 Get_Ultrasonic_Distance(void) {
    if (Get_Cur_Micros() - estimation_start >= TIMEOUT){
        IfxPort_setPinLow(P00_4_PORT, P00_4_PIN);
        Delay_Us(2);
        IfxPort_setPinHigh(P00_4_PORT, P00_4_PIN);
        Delay_Us(10);
        IfxPort_setPinLow(P00_4_PORT, P00_4_PIN);
        estimation_start = 0;
        estimations = 0;
    }
    return (float32)estimations / 58.8235f;
}

//uint32 Pulse_In(IfxPort_Pin pin, boolean state, uint32 timeout) {
//
//
//
//    // wait for any previous pulse to end
//    while (__getbit(&pin.port->IN.U, pin.pinIndex) == state) {
//        if (Get_Cur_Micros() - estimation_start >= timeout) {
//            return 0;
//        }
//    }
//
//    // wait for the pulse to start
//    while (__getbit(&pin.port->IN.U, pin.pinIndex) != state) {
//        if (Get_Cur_Micros() - estimation_start >= timeout) {
//            return 0;
//        }
//    }
//
//    // wait for the pulse to stop
//    uint32 estimation_start = Get_Cur_Micros();
//    while (__getbit(&pin.port->IN.U, pin.pinIndex) == state) {
//        if (Get_Cur_Micros() - estimation_start >= timeout) {
//            return 0;
//        }
//    }
//
//
//}

//uint32 Pulse_In(IfxPort_Pin pin, boolean state, uint32 timeout) {
//    //initExtIrq(pin, uint8 trig_mode, uint8 priority);
//    uint32 start_micros = Get_Cur_Micros();
//
//
//    IFX_INTERRUPT(ISR0, 0, 0x10);
//    uint32 ISR0(void){
//        uint32 estimation_end = Get_Cur_Micros();
//
//        return (estimation_end - estimation_start);
//
//    }
//    // wait for any previous pulse to end
//
//    while (__getbit(&pin.port->IN.U, pin.pinIndex) == state) {
//        if (Get_Cur_Micros() - start_micros >= timeout) {
//            return 0;
//        }
//    }
//
//    // wait for the pulse to start
//    while (__getbit(&pin.port->IN.U, pin.pinIndex) != state) {
//        if (Get_Cur_Micros() - start_micros >= timeout) {
//            return 0;
//        }
//    }
//
//    // wait for the pulse to stop
//    uint32 estimation_start = Get_Cur_Micros();
//    while (__getbit(&pin.port->IN.U, pin.pinIndex) == state) {
//        if (Get_Cur_Micros() - start_micros >= timeout) {
//            return 0;
//        }
//    }
//
//}
