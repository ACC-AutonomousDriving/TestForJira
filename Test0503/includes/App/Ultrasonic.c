/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ultrasonic.h"

#include "Platform_Types.h"
#include "IfxPort.h"
#include "IfxCpu.h"
#include "IfxStm.h"

#include "Delay.h"
#include "extIrq.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define TIMEOUT     23529

#define P00_4_IRQ      IfxScu_REQ7_P00_4_IN //Trigger
#define Echo_rsring         &MODULE_P00,4

#define P11_10_IRQ      IfxScu_REQ12_P11_10_IN //Echo
#define Echo_falling         &MODULE_P11,10

#define Trigger         &MODULE_P00,8

/*********************************************************************************************************************/
/*---------------------------------------------global variables----------------------------------------------*/
/*********************************************************************************************************************/
volatile uint32 estimation_start;
volatile uint32 estimation_end;
volatile uint32 estimations;
/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
//uint32 Pulse_In(IfxPort_Pin pin, uint8 state, uint32 timeout);


/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(ISR_Rising, 0, 12);
IFX_INTERRUPT(ISR_Falling, 0, 14);
void ISR_Rising(void){ //rising
    estimation_end = Get_Cur_Micros();
    estimations = (estimation_end - estimation_start);
}
//rising 일띠ㅐ 0부터 세고  falinig일때 저장, 단, TIMEOUT 됐을 시 falining 인터럽트에서 저장 0으로 해주기
void ISR_Falling(void){ //falling

    IfxPort_setPinLow(Trigger);
    Delay_Us(2);
    IfxPort_setPinHigh(Trigger);
    Delay_Us(10);
    IfxPort_setPinLow(Trigger);
    estimation_start = Get_Cur_Micros();
}

void Init_Ultrasonic(void) {
    IfxPort_setPinModeOutput(Trigger, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeInput(Echo_falling, IfxPort_InputMode_pullUp);
    IfxPort_setPinModeInput(Echo_rsring, IfxPort_InputMode_pullUp);

    initExtIrq(P11_10_IRQ, risingEdgeDetection, 12);
    initExtIrq(P00_4_IRQ, fallingEdgeDetection, 14);
}

float32 Get_Ultrasonic_Distance(void) {
    if (Get_Cur_Micros() - estimation_start >= TIMEOUT){
        IfxPort_setPinLow(Trigger);
        Delay_Us(2);
        IfxPort_setPinHigh(Trigger);
        Delay_Us(10);
        IfxPort_setPinLow(Trigger);
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
