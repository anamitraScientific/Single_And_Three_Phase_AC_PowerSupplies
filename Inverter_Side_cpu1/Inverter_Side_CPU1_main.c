/*
 * Inverter_Side_CPU1_main.c
 *
 *  Created on: 05-Apr-2025
 *      Author: Anamitra Sarkar
 */

#include "NPC_Solution.h"
#include "device.h"
#include "hw_ipc.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "Config.h"
#include "Control_Variables.h"
#include "Common_Memmap.h"
#include "Ref_Gen.h"
#include "emavg.h"

// Variable declarations for state machine
void (*alpha_State_Ptr)(void);
void (*A_Task_Ptr)(void);


volatile SharedFlags cpu1Read_Flags;
volatile int cpu1Write_Flag;
volatile uint16_t cpu1Read[MAX_LENGTH];
volatile uint16_t cpu1Write[MAX_LENGTH] = {0x0000};


volatile signed int BaseLUT_A[LUT_SIZE];
volatile signed int BaseLUT_B[LUT_SIZE];
volatile signed int BaseLUT_C[LUT_SIZE];
volatile float32_t HarmonicA[MAX_HARMONIC_NO][3];
volatile float32_t HarmonicB[MAX_HARMONIC_NO][3];
volatile float32_t HarmonicC[MAX_HARMONIC_NO][3];


#pragma DATA_SECTION(cpu1Read, "SHARERAMGS6");
#pragma DATA_SECTION(cpu1Write, "SHARERAMGS7");
#pragma DATA_SECTION(cpu1Read_Flags,"SHARERAMGS8");
#pragma DATA_SECTION(cpu1Write_Flag,"SHARERAMGS9");

#pragma DATA_SECTION(BaseLUT_A, "SHARERAMGS10");
#pragma DATA_SECTION(BaseLUT_B, "SHARERAMGS10");
#pragma DATA_SECTION(BaseLUT_C, "SHARERAMGS10");
#pragma DATA_SECTION(HarmonicA, "SHARERAMGS10");
#pragma DATA_SECTION(HarmonicB, "SHARERAMGS10");
#pragma DATA_SECTION(HarmonicC, "SHARERAMGS10");


volatile uint16_t handshake;
volatile bool handshake_flag;

volatile uint16_t seqIndex;
volatile bool seq_run;
volatile float32_t seq_time;
volatile float32_t volt_step_rise_time;
volatile float32_t freq_step_rise_time;
volatile float32_t total_step_time;
volatile uint32_t total_timer_count_per_step;
volatile uint32_t timerCount;


EMAVG vout;
EMAVG Vdc_out;

float32_t H_alpha = 0.9999f;
float32_t updateRate = 1.0f;

// State Machine function prototypes
void A0(void); // Alpha states
void A1(void); // A branch states


// Function prototypes
static inline void CheckSharedMemory(void);


void main(void)
{

//    Harmonic_Array_Init();

    /* Electrical angle step */
    Angle_Step = TWO_PI / (float)LUT_SIZE;

    float32_t Tswitching;
    Tswitching = 1.0f/(float)Fswitching;
//
//    POWER_MEAS_SINE_ANALYZER_reset(&PPA_phaseA);
////    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, Tswitching, 0.5f, 600U);
//    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, Tswitching);

    EMAVG_config(&vout, 0.05);
    EMAVG_config(&Vdc_out, 0.05);


//    POWER_MEAS_SINE_ANALYZER_reset(&PPA_phaseA);
//    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, Fswitching, 0.05f, 1000.0f, 25.0f);
    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, T_switching);

    SPLL_1PH_SOGI_config(&spll_line, AC_FREQ_HZ, ISR_FREQUENCY, (float32_t)(222.2862), (float32_t)(-222.034));

    SPLL_1PH_SOGI_reset(&V_line_pll);
//    SPLL_1PH_SOGI_config(&V_line_pll,
//                         AC_FREQ_HZ,
//                         ISR_FREQUENCY,
//                         (float32_t) 166.9743385,
//                         (float32_t) -166.2661165);
    SPLL_1PH_SOGI_config(&V_line_pll, AC_FREQ_HZ, ISR_FREQUENCY, (float32_t)(222.2862), (float32_t)(-222.034));
    SPLL_1PH_SOGI_coeff_calc(&V_line_pll);

    SPLL_1PH_SOGI_config(&sogi_Ia, AC_FREQ_HZ, ISR_FREQUENCY, (float32_t)(222.2862), (float32_t)(-222.034));

    V_fb[0] = 0.0f;
    V_fb[0] = 0.0f;
    I_fb[0] = 0.0f;
    I_fb[1] = 0.0f;

    NPC_HAL_setupDevice();

    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI, 2, SYSCTL_CPUSEL_CPU2); // Hand-over the SCI B module access to CPU2
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI, 3, SYSCTL_CPUSEL_CPU2); // Hand-over the SCI C module access to CPU2
    MemCfg_setGSRAMControllerSel(MEMCFG_SECT_GS6 | MEMCFG_SECT_GS8 | MEMCFG_SECT_GS10, MEMCFG_GSRAMCONTROLLER_CPU2); //Hand-over the control of GS6 & GS8 shared memory section to CPU2



    alpha_State_Ptr = &A0;           // Tasks State-machine init
    A_Task_Ptr = &A1;
    NPC_HAL_disablePWMCLKCounting(); // Stop all PWM mode clock

    NPC_HAL_SCI_B_GPIO_18_19_Init();
    NPC_HAL_SCI_C_GPIO_56_139_Init();


    NPC_HAL_configureDAC();
    NPC_HAL_setupADC();
    NPC_HAL_setupEPWM();
    NPC_globalVariablesInit();


    memset((void*)cpu1Read, 0, sizeof(cpu1Read));
    cpu1Read_Flags.VarAddr[0] = 0;
    cpu1Read_Flags.VarAddr[1] = 0;
    cpu1Read_Flags.dataLength[0] = 0;
    cpu1Read_Flags.dataLength[1] = 0;
    memset((void*)cpu1Write, 0, sizeof(cpu1Write));
    cpu1Write_Flag = 0;



    NPC_HAL_setupEPWMtoTriggerADCSOC();

    NPC_HAL_enablePWMCLKCounting();

    NPC_HAL_setGpioPinsAsPWM();
  //  NPC_HAL_setupGateDriverEnableGPIO();
    NPC_HAL_setupLEDGPIO();
    NPC_HAL_setupRelayGPIO();
    GPIO_writePin(67, 1);
#if CONVERTER_TYPE == THREE_PHASE

    NPC_HAL_TurnON_LED2();

#else
#endif

    NPC_HAL_setup_INVONOFF_GPIO();

    NPC_HAL_setupBoardProtection();
    NPC_HAL_setupInterrupt();
    Device_bootCPU2(BOOTMODE_BOOT_TO_FLASH_SECTOR0);//Send boot command to allow the CPU2 application to begin execution
    IPC_sync(IPC_CPU1_L_CPU2_R, IPC_FLAG31);

    EINT;
    ERTM;

    while(1)
    {
//        (*alpha_State_Ptr)();    // AUTO-START

//        NPC_HAL_passDAC_BVals(2048);
//        NPC_HAL_passDAC_AVals(4095);

        WriteMeasureDataToSharedMemory();
//        CheckSharedMemory();
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        if(seq_start == true)
//        {
//            StartPowerStage == 1;
//        }
//        else
//        {
//
//        }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(StartPowerStage == 1)
//        if(StartPowerStage == 1)
        {
            NPC_HAL_enablePWMCLKCounting();
        }
        else if(StartPowerStage == 0)
        {
//            NPC_HAL_disablePWMCLKCounting();
//            NPC_HAL_ForceOSTEVENTtoALLEPWM();

            EALLOW;
            AdcaRegs.ADCSOCFRC1.all = 0x00FF;
            AdcbRegs.ADCSOCFRC1.all = 0x00FF;
            AdccRegs.ADCSOCFRC1.all = 0x00FF;
            EDIS;
            NPC_readCurrentAndVoltageSignals();
        }

        if (Va_fb < 20.0f)
            Mff = 0.0f;
        else if (Va_fb < 140.0f)
            Mff = 0.2f;
        else if (Va_fb < 220.0f)
            Mff = 0.4f;
        else if (Va_fb < 280.0f)
            Mff = 0.6f;
        else if (Va_fb < 360.0f)
            Mff = 0.8f;
        else
        {

        }



    }

}





//  STATE-MACHINE SEQUENCING AND SYNCRONIZATION FOR SLOW BACKGROUND TASKS
void A0(void)
{
    // loop rate synchronizer for A-tasks
    if(NPC_GET_TASK_A_TIMER_OVERFLOW_STATUS == 1)
    {
        NPC_CLEAR_TASK_A_TIMER_OVERFLOW_FLAG;   // clear flag
        (*A_Task_Ptr)();                        // jump to an A Task
        vTimer0[0]++;                           // virtual timer 0, instance 0 (spare)
    }
    // Comment out to allow only A tasks
    alpha_State_Ptr = &A0;
}


//  A - TASKS (executed in every 1 msec)
void A1(void)
{
    // NPC_autoStart_INVERTER();  // INVERTER AUTOSTART
    // the next time CpuTimer0 'counter' reaches Period value go to A1
    A_Task_Ptr = &A1;
}






// MAIN ISR LOOP
interrupt void ISR(void)
{
    if(LoadSource_mode != 0)
    {
        RUN_INV_ISR_LoadMode();
    }
    else
    {
        RUN_INV_ISR_SourceMode();
//        RUN_INV_ISR_SourceMode_CC_Loop();
    }



#if CONVERTER_TYPE == SINGLE_PHASE
    NPC_HAL_clear_ISR1_EPWM1_InterruptFlag();
#elif CONVERTER_TYPE == THREE_PHASE
    NPC_HAL_clear_ISR1_EPWM4_InterruptFlag();
#else
#endif


    NPC_HAL_Acknowledge_Interrupt();
}



static inline void CheckSharedMemory(void)
{
//    if(cpu1Read_Flags.EEPROM_STM_cpu1Read == TRUE){
//        MemDataUpdate();
//    }
//    else{}
//
//    uint16_t Start_Address;
//    Start_Address = cpu1Read_Flags.readVarAddr[0]*256 + cpu1Read_Flags.readVarAddr[1];
//    MarkDirty(Start_Address);
    ReadingDataFromSharedMemory();
}

