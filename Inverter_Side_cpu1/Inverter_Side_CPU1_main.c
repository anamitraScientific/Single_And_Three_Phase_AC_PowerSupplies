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

//    float32_t Tswitching;
//    Tswitching = 1.0f/(float)Fswitching;
//
//    POWER_MEAS_SINE_ANALYZER_reset(&PPA_phaseA);
////    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, Tswitching, 0.5f, 600U);
//    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, Tswitching);

    POWER_MEAS_SINE_ANALYZER_reset(&PPA_phaseA);
    POWER_MEAS_SINE_ANALYZER_config(&PPA_phaseA, Fswitching, 0.05f, 1000.0f, 25.0f);


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

#if mode1 == testing
    NPC_Testing();
#else
#endif

#if mode1 == GE_AC
    #if AC_submode == NormalOperation
            RUN_INV_ISR_ABC();

    #elif AC_submode == ACFaults
            RUN_INV_ISR_ABC();  // applicable in balanced and CV mode

    #elif AC_submode == harmonic_injection
            RUN_INV_ISR_GE_AC_fundamental_with_harmonics();

    #elif AC_submode == arbitary_frequency
            //RUN_INV_ISR_GE_AC_arbitary_frequency();
            RUN_INV_ISR_GE_AC_fundamental_with_harmonics();  //select unbalanced mode; provide frequency in multiples of 50 working upto 1khz from watch window
                                                             // but provide arb. freq. as 50hz for all phases in project defines

    #elif AC_submode == SINGLE_PHASE_paralleling
            RUN_INV_ISR_GE_AC_paralleling();                // not tested

    #elif AC_submode == programmable_output_impedence
            RUN_INV_ISR_GE_AC_programmable_output_impedence();

    #elif AC_submode == Arbitrary_waveform_mode
            //RUN_INV_ISR_GE_AC_arbitary_waveform();
            RUN_INV_ISR_GE_AC_fundamental_with_harmonics();
    #else
    #endif
#else
#endif

#if mode1 == EL_AC
#if loop == CC_loop
    #if AC_submode == NormalOperation
            //RUN_INV_ISR_EL_AC_fundamental();
            //RUN_INV_ISR_EL_LL(); for line to line
            RUN_INV_ISR_EL_AC_fundamental_with_harmonics();  // provide only fundamental

    #elif AC_submode == harmonic_injection
            RUN_INV_ISR_EL_AC_fundamental_with_harmonics();

    #elif AC_submode == arbitary_frequency
            //RUN_INV_ISR_EL_AC_arbitary_frequency();
            RUN_INV_ISR_EL_AC_fundamental_with_harmonics();  //select unbalanced mode; provide frequency in multiples of 50 working upto 1khz from watch window
                                                                         // but provide arb. freq. as 50hz for all phases in project defines.h

    #elif AC_submode == SINGLE_PHASE_paralleling
            RUN_INV_ISR_EL_AC_paralleling();                // not tested

    #elif AC_submode == programmable_output_impedence
            RUN_INV_ISR_EL_AC_programmable_output_impedence();

    #elif AC_submode == Arbitrary_waveform
            RUN_INV_ISR_EL_AC_arbitary_waveform();          // not tested
    #else
    #endif
#else
#endif
#else
#endif

#if mode1 == GE_DC
    #if DC_submode == NormalOperation
            RUN_INV_ISR_GE_DC();
            //RUN_INV_ISR_GE_DC_dual();  // not tested

    #elif DC_submode == Bipolar
            RUN_INV_ISR_GE_DC_Bipolar();

    #elif DC_submode == paralleling
            RUN_INV_ISR_GE_DC_paralleling();    // not tested

    #elif DC_submode == programmable_output_impedence_dc
            RUN_INV_ISR_GE_DC_programmable_output_impedence();
    #else
    #endif
#else
#endif

#if mode1 == EL_DC  // not tested
#if loop == CC_loop
    #if DC_submode == NormalOperation
            RUN_INV_ISR_GE_DC();

    #elif DC_submode == Bipolar
            RUN_INV_ISR_GE_DC_Bipolar();

    #elif DC_submode == paralleling
            RUN_INV_ISR_GE_DC_paralleling();

    #elif DC_submode == programmable_output_impedence_dc
            RUN_INV_ISR_GE_DC_programmable_output_impedence();
    #else
    #endif
#else
#endif
#else
#endif

#if mode1 == GE_AC_DC // not tested
    RUN_INV_ISR_GE_AC_fundamental();
#else
#endif

//    WriteMeasureDataToSharedMemory();

//    Run_aux_ISR();
//    WriteMeasureDataToSharedMemory();


#if CONVERTER_TYPE == SINGLE_PHASE
    NPC_HAL_clear_ISR1_EPWM1_InterruptFlag();
#elif CONVERTER_TYPE == THREE_PHASE
    NPC_HAL_clear_ISR1_EPWM4_InterruptFlag();
#else
#endif
    NPC_HAL_Acknowledge_Interrupt();
}



//AUX ISR related Function definitions
//interrupt void AUX_ISR(void)
//{
//    if(seq_start == true)
//    {
//        if(seq_run == false)
//        {
//            seqIndex = 0;
//            seq_run = true;
//
//            VA_fundamental = seq_table[seqIndex][0];
//            AC_Freq_Ref_A = seq_table[seqIndex][1];
//            seq_time = seq_table[seqIndex][2];
//
//            volt_step_rise_time = (float32_t)((1/ISR_FREQUENCY) * ((VA_fundamental - VA_RefSlewRamp.out_slew) / slope_VacRef));
//            freq_step_rise_time = (float32_t)((1/ISR_FREQUENCY) * ((AC_Freq_Ref_A - FreqRefSlewRamp_A.out_slew) / slope_FreqRef));
//
//            if(volt_step_rise_time >= freq_step_rise_time)
//            {
//                total_step_time = (volt_step_rise_time + seq_time) * 1000000; // seconds converted to micro seconds
//            }
//            else
//            {
//                total_step_time = (freq_step_rise_time + seq_time) * 1000000; // seconds converted to micro seconds
//            }
//
//            total_timer_count_per_step = (uint32_t)(total_step_time / 100);
//        }
//        else
//        {
//           if(timerCount == total_timer_count_per_step)
//           {
//               seqIndex++;
//               timerCount = 0;
//               seq_run = false;
//           }
//        }
//    }

//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}


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

