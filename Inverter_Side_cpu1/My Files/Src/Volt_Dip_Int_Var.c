/*
 * Volt_Dip_Int_Var.c
 *
 *  Created on: Feb 17, 2026
 *      Author: admin
 */

#include "Volt_Dip_Int_Var.h"

float32_t pll_ang_A_prev;
float32_t ratedVoltage;
float32_t reducedVolatge;
float32_t isrInterruptPerCycle;
float32_t InterruptCount;
float32_t restoringSlew;

void RUN_INV_ISR_DipsAndInterruptionSequence(void)
{
    if(StartSeq == 1)
    {
        RAMP_reset(&theta_A);
        pll_ang_A = 0.0f;
        pll_ang_A_prev = 0.0f;

        REFslew_reset(&VA_RefSlewRamp);

        err_VA = 0;
        StartSeq = 0;

        ratedVoltage = Vac_fundamental;
        reducedVolatge = Vac_fundamental * 0.7f;
        isrInterruptPerCycle = (float32_t)(ISR_FREQUENCY / AC_Freq_Ref);
        InterruptCount = isrInterruptPerCycle * 25.0f;
        restoringSlew =  (ratedVoltage - reducedVolatge)/InterruptCount;

        NPC_HAL_ClearALLPWMTripFlags();
    }


    NPC_readCurrentAndVoltageSignals();  // ADC FEEDBACK
    RAMP_run(&theta_A,AC_Freq_Ref,ISR_FREQUENCY);
    pll_ang_A = (float32_t)(theta_A.out*2*PI);
    Ref_Gen_function();

    if(pll_ang_A_prev > pll_ang_A)
    {
        cycleCount++;
    }
    else{}

    pll_ang_A_prev = pll_ang_A;


    if(cycleCount < 100)
    {
        REFslew_run(&VA_RefSlewRamp, ratedVoltage, 1.0f);
        VA_RefSlewed = VA_RefSlewRamp.out_slew;

        Va_ref = VA_RefSlewed*Ref_A;
        if(Va_ref > 300.0f) Va_ref = 300.0f;
        else if(Va_ref < -300.0f) Va_ref = -300.0f;

        err_VA = (float)(Va_ref - Va_fb);  // ERROR SIGNAL GIVEN TO CONTROLLER.
        uk_Va  = runPR_custom(&Testg1, err_VA);
        Ma1 = (float)(uk_Va + Va_fb)/(400);   // PR-CONTROLLER OUTPUT + FEEDFORWARD.
        NPC_Calculate_duty(Ma1);
        NPC_HAL_updatePWMDutyAndDeadBand(dutyA_S1_Ref,
                                         dutyB_S1_Ref,
                                         DeadBand);

        DeadBand = DBTicks-1;
        if(DeadBand >= DBTicks)
        {
            DeadBand = DeadBand - 1;
        }
    }

    else if(cycleCount >= 100 && cycleCount < 101)
    {
        REFslew_run(&VA_RefSlewRamp, reducedVolatge, 1.0f);
        VA_RefSlewed = VA_RefSlewRamp.out_slew;

        Va_ref = VA_RefSlewed*Ref_A;
        if(Va_ref > 300.0f) Va_ref = 300.0f;
        else if(Va_ref < -300.0f) Va_ref = -300.0f;

        err_VA = (float)(Va_ref - Va_fb);  // ERROR SIGNAL GIVEN TO CONTROLLER.
        uk_Va  = runPR_custom(&Testg1, err_VA);
        Ma1 = (float)(uk_Va + Va_fb)/(400);   // PR-CONTROLLER OUTPUT + FEEDFORWARD.
        NPC_Calculate_duty(Ma1);
        NPC_HAL_updatePWMDutyAndDeadBand(dutyA_S1_Ref,
                                         dutyB_S1_Ref,
                                         DeadBand);

        DeadBand = DBTicks-1;
        if(DeadBand >= DBTicks)
        {
            DeadBand = DeadBand - 1;
        }
    }

    else if(cycleCount >= 101 && cycleCount < 126)
    {
        REFslew_run(&VA_RefSlewRamp, ratedVoltage, restoringSlew);
        VA_RefSlewed = VA_RefSlewRamp.out_slew;

        Va_ref = VA_RefSlewed*Ref_A;
        if(Va_ref > 300.0f) Va_ref = 300.0f;
        else if(Va_ref < -300.0f) Va_ref = -300.0f;

        err_VA = (float)(Va_ref - Va_fb);  // ERROR SIGNAL GIVEN TO CONTROLLER.
        uk_Va  = runPR_custom(&Testg1, err_VA);
        Ma1 = (float)(uk_Va + Va_fb)/(400);   // PR-CONTROLLER OUTPUT + FEEDFORWARD.
        NPC_Calculate_duty(Ma1);
        NPC_HAL_updatePWMDutyAndDeadBand(dutyA_S1_Ref,
                                         dutyB_S1_Ref,
                                         DeadBand);

        DeadBand = DBTicks-1;
        if(DeadBand >= DBTicks)
        {
            DeadBand = DeadBand - 1;
        }
    }
    else if(cycleCount >= 126)
    {
        NPC_HAL_ForceOSTEVENTtoALLEPWM();
        StartSeq = 1;
        cycleCount = 0;
        StartVoltDipSeq = 0;
    }
}
