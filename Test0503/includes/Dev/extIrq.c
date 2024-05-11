/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "extIrq.h"
#include "IfxScuEru.h"
#include "IfxSrc_regdef.h"
#include "IfxSrc.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
static uint8 cnt_ogu = 0;

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void initExtIrq(irq_t pin, uint8 trig_mode, uint8 priority)
{
    boolean irq_state = IfxCpu_disableInterrupts();

    /* ERS의 EICR.EXIS 설정 (0~3의 값만 가능한 것으로 보아, 하나의 ERS당 하나의 input만 가능한 것으로 보임)*/
    IfxScuEru_initReqPin(&pin, IfxPort_InputMode_pullDown);

    /* ETL의 EICR.REN & EICR.FEN 설정 */
    if ((trig_mode == risingEdgeDetection) || (trig_mode == everyEdgeDetection))
    {
        IfxScuEru_enableRisingEdgeDetection(pin.channelId);
    }
    if ((trig_mode == fallingEdgeDetection) || (trig_mode == everyEdgeDetection))
    {
        IfxScuEru_enableFallingEdgeDetection(pin.channelId);
    }

    /* ETL의 EICR.EIEN 설정 */
    IfxScuEru_enableTriggerPulse(pin.channelId);
    /* ETL의 EICR.INP 설정 (OGU 선택)*/
    IfxScuEru_connectTrigger(pin.channelId, cnt_ogu);

    /* OGU의 IGCR.IGP 설정 */
    IfxScuEru_setInterruptGatingPattern(cnt_ogu, IfxScuEru_InterruptGatingPattern_alwaysActive);

    /* OGU에 해당하는 IR의 ICU 할당 */
    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int) cnt_ogu % 4];

    /* ers 8개, ogu 8개이므로 ogu 하나 당 ers 하나 할당 */
    cnt_ogu++;

    /* SRC.SRPN & SRC.TOS & SRC.SRE 설정 */
    IfxSrc_init(src, IfxSrc_Tos_cpu0, priority);
    IfxSrc_enable(src);

    IfxCpu_restoreInterrupts(irq_state);
}
