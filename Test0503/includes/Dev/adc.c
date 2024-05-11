/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "adc.h"
#include "IfxVadc_Adc.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
adc_t SAR0_0 = {0, 0};
adc_t SAR0_1 = {0, 1};
adc_t SAR0_2 = {0, 2};
adc_t SAR0_3 = {0, 3};

adc_t SAR2_4 = {2, 4};
adc_t SAR2_5 = {2, 5};

adc_t SAR3_0 = {3, 0};
adc_t SAR3_1 = {3, 1};

adc_t SAR4_4 = {4, 4};
adc_t SAR4_5 = {4, 5};
adc_t SAR4_6 = {4, 6};
adc_t SAR4_7 = {4, 7};

adc_t SAR5_4 = {5, 4};
adc_t SAR5_5 = {5, 5};
adc_t SAR5_6 = {5, 6};
adc_t SAR5_7 = {5, 7};

IfxVadc_Adc g_adc;
IfxVadc_Adc_Group g_adc_group[6];

boolean g_f_adc_init = FALSE;
boolean g_f_adc_group_init[6] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void initAdc(adc_t *pin)
{
    boolean irq_state = IfxCpu_disableInterrupts();

    if(g_f_adc_init == FALSE)
    {
        g_f_adc_init = TRUE;

        IfxVadc_Adc_Config adc_config;
        IfxVadc_Adc_initModuleConfig(&adc_config, &MODULE_VADC);
        IfxVadc_Adc_initModule(&g_adc, &adc_config);
    }

    uint8 group_id = pin->group_id;
    if(g_f_adc_group_init[group_id] == FALSE)
    {
        g_f_adc_group_init[group_id] = TRUE;

        IfxVadc_Adc_GroupConfig adc_group_config;
        IfxVadc_Adc_initGroupConfig(&adc_group_config, &g_adc);

        adc_group_config.groupId = group_id;
        adc_group_config.master = adc_group_config.groupId;

        adc_group_config.arbiter.requestSlotBackgroundScanEnabled = TRUE;
        adc_group_config.backgroundScanRequest.autoBackgroundScanEnabled = TRUE;

        adc_group_config.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

        IfxVadc_Adc_initGroup(&(g_adc_group[group_id]), &adc_group_config);
    }

    IfxVadc_Adc_ChannelConfig adc_chn_config;

    IfxVadc_Adc_initChannelConfig(&adc_chn_config, &(g_adc_group[group_id]));

    adc_chn_config.channelId = pin->chn_id;
    adc_chn_config.resultRegister = (IfxVadc_ChannelResult)(0);
    adc_chn_config.backgroundChannel = TRUE;

    IfxVadc_Adc_initChannel(&(pin->chn), &adc_chn_config);

    unsigned chn_enable_bit = (1 << adc_chn_config.channelId);
    unsigned mask = chn_enable_bit;
    IfxVadc_Adc_setBackgroundScan(&g_adc, &(g_adc_group[group_id]), chn_enable_bit, mask);

    IfxVadc_Adc_startBackgroundScan(&g_adc);

    IfxCpu_restoreInterrupts(irq_state);
}

uint16 getAdc(adc_t *pin)
{
    Ifx_VADC_RES conv_result;
    do
    {
        conv_result = IfxVadc_Adc_getResult(&(pin->chn));
    } while(!conv_result.B.VF);

    return conv_result.B.RESULT;
}
