/*
 * NPC_Hardware_Setup.c
 *
 *  Created on: 24-Jan-2026
 *      Author: Anamitra Sarkar
 */

#include "NPC_hardware_setup.h"
//#include "NPC_solution.h"

//
//Set up of device
//
void NPC_HAL_setupDevice(void)
{
    InitSysCtrl();
    InitGpio();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();



//    EALLOW;
//    PieVectTable.TIMER0_INT = &AUX_ISR;
//    EDIS;
//
//    InitCpuTimers();
//    ConfigCpuTimer(&CpuTimer0, 200, 1000);
//
//    CpuTimer0Regs.TCR.all = 0x4000;
//
//    IER |= M_INT1;
//    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

}
//
//Set up of ePWM peripherals
//

//
// INV Module A Leg_1
//
static inline void NPC_HAL_setupEPwm1(void)
{
    EALLOW;
    //
    // Setup TBCLK
    //
    EPwm1Regs.TBPRD = TimeBase;                                 // Set timer period
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;                     // Enable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm1Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;               // Select SYNCOSEL at TBCTR = ZERO event & EPWM4 module is chosen as master module in this project
    EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#elif defined(_LAUNCHXL_F28388D)

#if CONVERTER_TYPE == SINGLE_PHASE
    EPwm1Regs.EPWMSYNCOUTEN.bit.ZEROEN = 1;                     // Selecting the EPWM1 module to be the master module for single phase converter
#elif CONVERTER_TYPE == THREE_PHASE
    EPwm1Regs.EPWMSYNCINSEL.bit.SEL = 4;                        // Select EPWM4 sync signal to be accepted in EPWM1 // applicable for three phase converter
    EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#else
#endif
#endif
    //
    // Setup shadowing
    //
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
//    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM4A on CAU event
//    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM4A on CAD event
//    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM4B on CAU event
//    EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM4B on CAD event
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;                        // Clear PWM4A on CAU event
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;                          // Set PWM4A on CAD event
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;                          // Set PWM4B on CAU event
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;                        // Clear PWM4B on CAD event

    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm1Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm1Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM4A as Input to DeadBand Module
    EDIS;
}

//
// INV Module A Leg_2
//

static inline void NPC_HAL_setupEPwm2(void)
{
    EALLOW;
    //
    // Setup TBCLK
    //
    EPwm2Regs.TBPRD = TimeBase;                                 // Set timer period
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;                     // Enable phase loading
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm2Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;               // Select SYNCOSEL at TBCTR = ZERO event & EPWM4 module is chosen as master module in this project
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#elif defined(_LAUNCHXL_F28388D)
#if CONVERTER_TYPE == SINGLE_PHASE
    EPwm2Regs.EPWMSYNCINSEL.bit.SEL = 1;                        // Selecting the EPWM2 module to accept sync signal from EPWM1 module
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#elif CONVERTER_TYPE == THREE_PHASE
    EPwm2Regs.EPWMSYNCINSEL.bit.SEL = 4;                        // Select EPWM4 sync signal to be accepted in EPWM2 // applicable for three phase converter
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#else
#endif
#endif
    //
    // Setup shadowing
    //
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
//    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM4A on CAU event
//    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM4A on CAD event
//    EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM4B on CAU event
//    EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM4B on CAD event
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;                        // Clear PWM4A on CAU event
    EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;                          // Set PWM4A on CAD event
    EPwm2Regs.AQCTLB.bit.CAU = AQ_CLEAR;                          // Set PWM4B on CAU event
    EPwm2Regs.AQCTLB.bit.CAD = AQ_SET;                        // Clear PWM4B on CAD event

    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm2Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm2Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM4A as Input to DeadBand Module
    EDIS;
}

//
// INV Module B Leg_1
//

static inline void NPC_HAL_setupEPwm3(void)
{
    EALLOW;
    //
    // Setup TBCLK
    //
    EPwm3Regs.TBPRD = TimeBase;                                 // Set timer period
    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;                      // Enable phase loading
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm3Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;               // Select SYNCOSEL at TBCTR = ZERO event & EPWM4 module is chosen as master module in this project
    EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#elif defined(_LAUNCHXL_F28388D)
    EPwm3Regs.EPWMSYNCINSEL.bit.SEL = 4;                        // Select EPWM4 sync signal to be accepted in EPWM3 // applicable for three phase converter
    EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#endif
    //
    // Setup shadowing
    //
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM4A on CAU event
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM4A on CAD event
    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM4B on CAU event
    EPwm3Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM4B on CAD event
//    EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;                        // Clear PWM4A on CAU event
//    EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;                          // Set PWM4A on CAD event
//    EPwm3Regs.AQCTLB.bit.CAU = AQ_CLEAR;                          // Set PWM4B on CAU event
//    EPwm3Regs.AQCTLB.bit.CAD = AQ_SET;                        // Clear PWM4B on CAD event
    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm3Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm3Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM4A as Input to DeadBand Module
    EDIS;
}

//
// INV Module B Leg_2
//
static inline void NPC_HAL_setupEPwm4(void)
{
    EALLOW;
#if CONVERTER_TYPE == SINGLE_PHASE

    //
    // Setup TBCLK
    //
    EPwm4Regs.TBPRD = TimeBase_aux;                                 // Set timer period
    EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm4Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;                      // Enable phase loading
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm4Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;               // Select SYNCOSEL at TBCTR = ZERO event & EPWM4 module is chosen as master module in this project
#elif defined(_LAUNCHXL_F28388D)
    EPwm4Regs.EPWMSYNCINSEL.bit.SEL = 1;                        // Selecting the EPWM2 module to accept sync signal from EPWM1 module
    EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#endif
    //
    // Setup shadowing
    //
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM4A on CAU event
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM4A on CAD event
    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM4B on CAU event
    EPwm4Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM4B on CAD event

    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm4Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm4Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM4A as Input to DeadBand Module


#elif CONVERTER_TYPE == THREE_PHASE

    //
    // Setup TBCLK
    //
    EPwm4Regs.TBPRD = TimeBase;                                 // Set timer period
    EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm4Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;                      // Enable phase loading
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm4Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;               // Select SYNCOSEL at TBCTR = ZERO event & EPWM4 module is chosen as master module in this project
#elif defined(_LAUNCHXL_F28388D)
    EPwm4Regs.EPWMSYNCOUTEN.bit.ZEROEN = 1;                     // Select EPWM4 to generate SYNC signal for other slave ePWM modules when TBCTR = 0; thus in other Words ePWM4 is selected as Master
#endif
    //
    // Setup shadowing
    //
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM4A on CAU event
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM4A on CAD event
    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM4B on CAU event
    EPwm4Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM4B on CAD event
//    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;                        // Clear PWM4A on CAU event
//    EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;                          // Set PWM4A on CAD event
//    EPwm4Regs.AQCTLB.bit.CAU = AQ_CLEAR;                          // Set PWM4B on CAU event
//    EPwm4Regs.AQCTLB.bit.CAD = AQ_SET;                        // Clear PWM4B on CAD event

    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm4Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm4Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM4A as Input to DeadBand Module

#endif
    EDIS;
}

#if CONVERTER_TYPE == THREE_PHASE
//
// INV Module C Leg_1
//
static inline void NPC_HAL_setupEPwm5(void)
{
    EALLOW;
    //
    // Setup TBCLK
    //
    EPwm5Regs.TBPRD = TimeBase;                                 // Set timer period
    EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm5Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;                      // Enable phase loading
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;                  // Select SYNCOSEL as SYNC_IN, so that the module is in SYNC with the master module
    EPwm5Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#elif defined(_LAUNCHXL_F28388D)
    EPwm5Regs.EPWMSYNCINSEL.bit.SEL = 4;                        // Selecting EPWM4_SYNCOUT signal as EPWMxSYNCIN to sync this module with the master module
    EPwm5Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#endif


    //
    // Setup shadowing
    //
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM5A on CAU event
    EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM5A on CAD event
    EPwm5Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM5B on CAU event
    EPwm5Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM5B on CAD event
//    EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;                        // Clear PWM4A on CAU event
//    EPwm5Regs.AQCTLA.bit.CAD = AQ_CLEAR;                          // Set PWM4A on CAD event
//    EPwm5Regs.AQCTLB.bit.CAU = AQ_CLEAR;                          // Set PWM4B on CAU event
//    EPwm5Regs.AQCTLB.bit.CAD = AQ_SET;                        // Clear PWM4B on CAD event

    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm5Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm5Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM5A as Input to DeadBand Module
    EDIS;
}
//
// INV Module C Leg_2
//
static inline void NPC_HAL_setupEPwm6(void)
{
    EALLOW;
    //
    // Setup TBCLK
    //
    EPwm6Regs.TBPRD = TimeBase;                                 // Set timer period
    EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;                         // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                                   // Clear counter

    //
    // Setup Counter Mode
    //
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;              // Count up_down
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;                      // Enable phase loading
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;                       // Clock ratio to SYSCLKOUT
#ifdef _LAUNCHXL_F28379D
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;                  // Select SYNCOSEL as SYNC_IN, so that the module is in SYNC with the master module
    EPwm6Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#elif defined(_LAUNCHXL_F28388D)
    EPwm6Regs.EPWMSYNCINSEL.bit.SEL = 4;                        // Selecting EPWM4_SYNCOUT signal as EPWMxSYNCIN to sync this module with the master module
    EPwm6Regs.TBCTL.bit.PHSDIR = TB_UP;                         // Select Counter direction to count Up after Synchronization event
#endif


    //
    // Setup shadowing
    //
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                 // Enable Shadow load Mode
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;               // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set Action Qualifier Regs
    //
    EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM5A on CAU event
    EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM5A on CAD event
    EPwm6Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM5B on CAU event
    EPwm6Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM5B on CAD event
//    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;                        // Clear PWM5A on CAU event
//    EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;                          // Set PWM5A on CAD event
//    EPwm6Regs.AQCTLB.bit.CAU = AQ_CLEAR;                          // Set PWM5B on CAU event
//    EPwm6Regs.AQCTLB.bit.CAD = AQ_SET;                        // Clear PWM5B on CAD event

    //
    // Active HIC PWMs - Setup DeadBand
    //
    EPwm6Regs.DBRED.bit.DBRED = DeadBandMax;                    //Set DBRED value
    EPwm6Regs.DBFED.bit.DBFED = DeadBandMax;                    //Set DBFED value
    EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM5A as Input to DeadBand Module
    EDIS;
}

#endif

static inline void NPC_HAL_setup_FAN_EPwm(void)
{
    EPwm8Regs.TBPRD = 2000;                                    // Set timer period
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;                        // Phase is 0
    EPwm8Regs.TBCTR = 0x0000;                                  // Clear counter

    //
    // Setup TBCLK
    //
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;             // Count up down
    EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE;                    // Disable phase loading
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                   // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                // Enable Shadow load Mode
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                // Enable Shadow load Mode
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;              // Load CMP registers on every TBCTR  = ZERO event
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;              // Load CMP registers on every TBCTR  = ZERO event

    //
    // Set actions
    //
    EPwm8Regs.AQCTLA.bit.CAU = AQ_CLEAR;                        // Clear PWM8A on CAU event
    EPwm8Regs.AQCTLA.bit.CAD = AQ_SET;                          // Set PWM8A on CAD event
    EPwm8Regs.AQCTLB.bit.CAU = AQ_SET;                          // Set PWM8B on CAU event
    EPwm8Regs.AQCTLB.bit.CAD = AQ_CLEAR;                        // Clear PWM8B on CAD event
    EPwm8Regs.CMPA.bit.CMPA   = (0.4 * 2000);                   // Compare value for duty cycle generation

    //
    // Active HIC PWMs - Setup Deadband
    //
    EPwm8Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;              //Enable DeadBand module
    EPwm8Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                   //Select Out mode as Active-High Complementary
    EPwm8Regs.DBCTL.bit.IN_MODE = DBA_ALL;                      //Select EPWM8A as Input to DeadBand Module

}

void NPC_HAL_setupEPWM(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE
    NPC_HAL_setupEPwm1();
    NPC_HAL_setupEPwm2();
    NPC_HAL_setupEPwm3();
    NPC_HAL_setupEPwm4();
    NPC_HAL_setup_FAN_EPwm();
#elif CONVERTER_TYPE == THREE_PHASE
    NPC_HAL_setupEPwm1();
    NPC_HAL_setupEPwm2();
    NPC_HAL_setupEPwm3();
    NPC_HAL_setupEPwm4();
    NPC_HAL_setupEPwm5();
    NPC_HAL_setupEPwm6();
#else
#endif
}

//
//Set up of ADC peripherals
//
static inline void NPC_HAL_ConfigureADC(void)
{
    EALLOW;

    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = ADCPrescaler; //set ADCCLK divider to /4
    AdcbRegs.ADCCTL2.bit.PRESCALE = ADCPrescaler;
    AdccRegs.ADCCTL2.bit.PRESCALE = ADCPrescaler;
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //
    //Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    //power up the ADC
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //
    //delay for 1ms to allow ADC time to power up
    //
    DELAY_US(1000);

    EDIS;
}

//
// Hall sensor outputs of Converter currents Ia, Ib &Ic
// are connected to ADCIN_A4, ADCIN_B4 & ADCIN_C4 pins respectively
//
static inline void NPC_HAL_setupI_convADC(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE

    EALLOW;

    AdcbRegs.ADCSOC0CTL.bit.CHSEL = IconvSense_channel;                     //SOC0 of ADC_B2 will convert I_conv connected to pin B2
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = ADCSOCTrigger1;                       //trigger on ePWM1 SOCA

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = IconvSense_channel;                     //SOC1 of ADC_B2 will convert I_conv connected to pin B2
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = ADCSOCTrigger2;                       //trigger on ePWM1 SOCB @TBCTR = CTR_PRD

    AdcbRegs.ADCSOC2CTL.bit.CHSEL = IconvSense_channel;                     //SOC2 of ADC_B2 will convert I_conv connected to pin B2
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = ADCSOCTrigger3;                       //trigger on ePWM3 SOCA @TBCTR_U_CMPB, CMPB = TB/2

    AdcbRegs.ADCSOC3CTL.bit.CHSEL = IconvSense_channel;                     //SOC3 of ADC_B2 will convert VAC connected to pin A2
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = ADCSOCTrigger4;                       //trigger on ePWM3 SOCB @TBCTR_U_CMPB, CMPB = ACQPS/2

    EDIS;

#elif CONVERTER_TYPE == THREE_PHASE

    EALLOW;
    /******************Iconv_A sensing(Sampled 4 times)**********************/

    AdcaRegs.ADCSOC0CTL.bit.CHSEL   = IconvSense_channel;  //SOC0 of ADC_A will convert Iconv_A connected to pin A4
    AdcaRegs.ADCSOC0CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdcaRegs.ADCSOC1CTL.bit.CHSEL   = IconvSense_channel;  //SOC1 of ADC_A will convert Iconv_A connected to pin A4
    AdcaRegs.ADCSOC1CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdcaRegs.ADCSOC2CTL.bit.CHSEL   = IconvSense_channel;  //SOC2 of ADC_A will convert Iconv_A connected to pin A4
    AdcaRegs.ADCSOC2CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdcaRegs.ADCSOC3CTL.bit.CHSEL   = IconvSense_channel;  //SOC3 of ADC_A will convert Iconv_A connected to pin A4
    AdcaRegs.ADCSOC3CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    /******************Iconv_B sensing(Sampled 4 times)**********************/

    AdcbRegs.ADCSOC0CTL.bit.CHSEL   = IconvSense_channel;  //SOC0 of ADC_B will convert Iconv_B connected to pin B4
    AdcbRegs.ADCSOC0CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdcbRegs.ADCSOC1CTL.bit.CHSEL   = IconvSense_channel;  //SOC1 of ADC_B will convert Iconv_B connected to pin B4
    AdcbRegs.ADCSOC1CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdcbRegs.ADCSOC2CTL.bit.CHSEL   = IconvSense_channel;  //SOC2 of ADC_B will convert Iconv_B connected to pin B4
    AdcbRegs.ADCSOC2CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdcbRegs.ADCSOC3CTL.bit.CHSEL   = IconvSense_channel;  //SOC3 of ADC_B will convert Iconv_B connected to pin B4
    AdcbRegs.ADCSOC3CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    /******************Iconv_C sensing(Sampled 4 times)**********************/

    AdccRegs.ADCSOC0CTL.bit.CHSEL   = IconvSense_channel;  //SOC0 of ADC_C will convert Iconv_C connected to pin C4
    AdccRegs.ADCSOC0CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdccRegs.ADCSOC1CTL.bit.CHSEL   = IconvSense_channel;  //SOC1 of ADC_C will convert Iconv_C connected to pin C4
    AdccRegs.ADCSOC1CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdccRegs.ADCSOC2CTL.bit.CHSEL   = IconvSense_channel;  //SOC2 of ADC_C will convert Iconv_C connected to pin C4
    AdccRegs.ADCSOC2CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdccRegs.ADCSOC3CTL.bit.CHSEL   = IconvSense_channel;  //SOC3 of ADC_C will convert Iconv_C connected to pin C4
    AdccRegs.ADCSOC3CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    EDIS;

#else
#endif
}

//
// AMC outputs of Grid Voltages Va, Vb &Vc
// are connected to ADCIN_A3, ADCIN_B3 & ADCIN_C3 pins respectively
//
static inline void NPC_HAL_setupV_gridADC(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE

    EALLOW;

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = VgridSense_channel;                       //SOC0 of ADC_A2 will convert VAC connected to pin A2
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADCSOCTrigger1;                       //trigger on ePWM1 SOCA

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = VgridSense_channel;                       //SOC1 of ADC_A2 will convert VAC connected to pin A2
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = ADCSOCTrigger2;  //ADCSOCTrigger1;                       //trigger on ePWM1 SOCB @TBCTR = CTR_PRD

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = VgridSense_channel;                       //SOC2 of ADC_A2 will convert VAC connected to pin A2
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = ADCSOCTrigger3;   //ADCSOCTrigger1;                       //trigger on ePWM3 SOCA @TBCTR_U_CMPB, CMPB = TB/2

    AdcaRegs.ADCSOC3CTL.bit.CHSEL = VgridSense_channel;                       //SOC3 of ADC_A2 will convert VAC connected to pin A2
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = ADCSOCTrigger4;   //ADCSOCTrigger1;                       //trigger on ePWM3 SOCB @TBCTR_U_CMPB, CMPB = ACQPS/2

    EDIS;

#elif CONVERTER_TYPE == THREE_PHASE

    EALLOW;
    /******************Vgrid_A sensing(Sampled 4 times)**********************/

    AdcaRegs.ADCSOC4CTL.bit.CHSEL   = VgridSense_channel;  //SOC4 of ADC_A will convert Vgrid_A connected to pin A3
    AdcaRegs.ADCSOC4CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdcaRegs.ADCSOC5CTL.bit.CHSEL   = VgridSense_channel;  //SOC5 of ADC_A will convert Vgrid_A connected to pin A3
    AdcaRegs.ADCSOC5CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdcaRegs.ADCSOC6CTL.bit.CHSEL   = VgridSense_channel;  //SOC6 of ADC_A will convert Vgrid_A connected to pin A3
    AdcaRegs.ADCSOC6CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdcaRegs.ADCSOC7CTL.bit.CHSEL   = VgridSense_channel;  //SOC7 of ADC_A will convert Vgrid_A connected to pin A3
    AdcaRegs.ADCSOC7CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC7CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    /******************Vgrid_B sensing(Sampled 4 times)**********************/
    AdcbRegs.ADCSOC4CTL.bit.CHSEL   = VgridSense_channel;  //SOC4 of ADC_B will convert Vgrid_B connected to pin B3
    AdcbRegs.ADCSOC4CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdcbRegs.ADCSOC5CTL.bit.CHSEL   = VgridSense_channel;  //SOC5 of ADC_B will convert Vgrid_B connected to pin B3
    AdcbRegs.ADCSOC5CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdcbRegs.ADCSOC6CTL.bit.CHSEL   = VgridSense_channel;  //SOC6 of ADC_B will convert Vgrid_B connected to pin B3
    AdcbRegs.ADCSOC6CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC6CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdcbRegs.ADCSOC7CTL.bit.CHSEL   = VgridSense_channel;  //SOC7 of ADC_B will convert Vgrid_B connected to pin B3
    AdcbRegs.ADCSOC7CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC7CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    /******************Vgrid_C sensing(Sampled 4 times)**********************/
    AdccRegs.ADCSOC4CTL.bit.CHSEL   = VgridSense_channel;  //SOC4 of ADC_C will convert Vgrid_C connected to pin C3
    AdccRegs.ADCSOC4CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdccRegs.ADCSOC5CTL.bit.CHSEL   = VgridSense_channel;  //SOC5 of ADC_C will convert Vgrid_C connected to pin C3
    AdccRegs.ADCSOC5CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdccRegs.ADCSOC6CTL.bit.CHSEL   = VgridSense_channel;  //SOC6 of ADC_C will convert Vgrid_C connected to pin C3
    AdccRegs.ADCSOC6CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC6CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdccRegs.ADCSOC7CTL.bit.CHSEL   = VgridSense_channel;  //SOC7 of ADC_C will convert Vgrid_C connected to pin C3
    AdccRegs.ADCSOC7CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC7CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    EDIS;

#else
#endif
}

//
// AMC outputs of DC Link Voltages VDC_1 & VDC_2
// are connected to ADCIN_A2 & ADCIN_B2 pins respectively
//
static inline void NPC_HAL_setupVDC_ADC(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE

    EALLOW;

    AdccRegs.ADCSOC0CTL.bit.CHSEL = Vdc_1Sense_channel;                      //SOC0 of ADC_C2 will convert V_DC connected to pin C2
    AdccRegs.ADCSOC0CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = ADCSOCTrigger1;                       //trigger on ePWM1 SOCA

    AdccRegs.ADCSOC1CTL.bit.CHSEL = Vdc_1Sense_channel;                      //SOC1 of ADC_C2 will convert V_DC connected to pin C2
    AdccRegs.ADCSOC1CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = ADCSOCTrigger2;                       //trigger on ePWM1 SOCB @TBCTR = CTR_PRD

    AdccRegs.ADCSOC2CTL.bit.CHSEL = Vdc_1Sense_channel;                      //SOC2 of ADC_C2 will convert V_DC connected to pin C2
    AdccRegs.ADCSOC2CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = ADCSOCTrigger3;                       //trigger on ePWM3 SOCA @TBCTR_U_CMPB, CMPB = TB/2

    AdccRegs.ADCSOC3CTL.bit.CHSEL = Vdc_1Sense_channel;                      //SOC3 of ADC_C2 will convert V_DC connected to pin C2
    AdccRegs.ADCSOC3CTL.bit.ACQPS = Acqps;                                  //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = ADCSOCTrigger4;                       //trigger on ePWM3 SOCB @TBCTR_U_CMPB, CMPB = ACQPS/2

    EDIS;

#elif CONVERTER_TYPE == THREE_PHASE

    EALLOW;
    /******************VDC_1sensing(Sampled 4 times)**********************/

    AdcaRegs.ADCSOC8CTL.bit.CHSEL   = Vdc_1Sense_channel;  //SOC8 of ADC_A will convert VDC_1 connected to pin A2
    AdcaRegs.ADCSOC8CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC8CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdcaRegs.ADCSOC9CTL.bit.CHSEL   = Vdc_1Sense_channel;  //SOC9 of ADC_A will convert VDC_1 connected to pin A2
    AdcaRegs.ADCSOC9CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC9CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdcaRegs.ADCSOC10CTL.bit.CHSEL   = Vdc_1Sense_channel;  //SOC10 of ADC_A will convert VDC_1 connected to pin A2
    AdcaRegs.ADCSOC10CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC10CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdcaRegs.ADCSOC11CTL.bit.CHSEL   = Vdc_1Sense_channel;  //SOC11 of ADC_A will convert VDC_1 connected to pin A2
    AdcaRegs.ADCSOC11CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC11CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    /******************VDC_2sensing(Sampled 4 times)**********************/
    AdcbRegs.ADCSOC8CTL.bit.CHSEL   = Vdc_2Sense_channel;  //SOC8 of ADC_B will convert VDC_2 connected to pin B2
    AdcbRegs.ADCSOC8CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC8CTL.bit.TRIGSEL = ADCSOCTrigger1;      //trigger on ePWM4 SOCA @TBCTR_D_CMPB, CMPB = ACQPS/2

    AdcbRegs.ADCSOC9CTL.bit.CHSEL   = Vdc_2Sense_channel;  //SOC9 of ADC_B will convert VDC_2 connected to pin B2
    AdcbRegs.ADCSOC9CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC9CTL.bit.TRIGSEL = ADCSOCTrigger2;      //trigger on ePWM5 SOCA @TBCTR_U_CMPB, CMPB = TBPRD/4

    AdcbRegs.ADCSOC10CTL.bit.CHSEL   = Vdc_2Sense_channel;  //SOC10 of ADC_B will convert VDC_2 connected to pin B2
    AdcbRegs.ADCSOC10CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC10CTL.bit.TRIGSEL = ADCSOCTrigger3;      //trigger on ePWM5 SOCB @TBCTR_D_CMPB, CMPB = TBPRD/4

    AdcbRegs.ADCSOC11CTL.bit.CHSEL   = Vdc_2Sense_channel;  //SOC11 of ADC_B will convert VDC_2 connected to pin B2
    AdcbRegs.ADCSOC11CTL.bit.ACQPS   = Acqps;               //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC11CTL.bit.TRIGSEL = ADCSOCTrigger4;      //trigger on ePWM4 SOCB @TBCTR_PRD

    EDIS;

#else
#endif
}


void NPC_HAL_setupADC(void)
{
    NPC_HAL_ConfigureADC();
    NPC_HAL_setupI_convADC();
    NPC_HAL_setupV_gridADC();
    NPC_HAL_setupVDC_ADC();
}

//
//Set up of DAC peripherals
//
void NPC_HAL_configureDAC(void)
{
    EALLOW;
    DacaRegs.DACCTL.bit.DACREFSEL = 1;
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacaRegs.DACVALS.all = 0;

    DacbRegs.DACCTL.bit.DACREFSEL = 1;
    DacbRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacbRegs.DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up
    EDIS;
}
void NPC_HAL_passDAC_AVals(uint16_t DACval_A)
{
    EALLOW;
    DacaRegs.DACVALS.all = DACval_A;
    EDIS;
}
void NPC_HAL_passDAC_BVals(uint16_t DACval_B)
{
    EALLOW;
    DacbRegs.DACVALS.all = DACval_B;
    EDIS;
}

//
//ADC SOC trigger setup
//
void NPC_HAL_setupEPWMtoTriggerADCSOC(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE
    //
    // setup EPWM1_ADCSOCA as ADCSOCTrigger1
    //
    EPwm1Regs.CMPB.bit.CMPB     = (Uint16)(Acqps*0.5f);     // CMPB = ACQPS/2
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTRD_CMPB;             // Select SOCA on TBCTR_D = CMPB Event
    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;                        // Enable SOCA
    EPwm1Regs.ETPS.bit.SOCAPRD  = ET_1ST;                   // Generate SOCA on 1st event
    //
    // setup EPWM3_ADCSOCA as ADCSOCTrigger2
    //
    EPwm3Regs.CMPB.bit.CMPB     = (Uint16)(TimeBase/4);  // CMPB = TimeBase/4
    EPwm3Regs.ETSEL.bit.SOCASEL = ET_CTRD_CMPB;              // Select SOCA on TBCTR_U = CMPB Event
    EPwm3Regs.ETSEL.bit.SOCAEN  = 1;                         // Enable SOCA
    EPwm3Regs.ETPS.bit.SOCAPRD  = ET_1ST;                    // Generate SOCA on 1st event
    //
    // setup EPWM3_ADCSOCB as ADCSOCTrigger3
    //
    EPwm3Regs.ETSEL.bit.SOCBSEL = ET_CTRU_CMPB;              // Select SOCB on TBCTR_D = CMPB Event
    EPwm3Regs.ETSEL.bit.SOCBEN  = 1;                         // Enable SOCB
    EPwm3Regs.ETPS.bit.SOCBPRD  = ET_1ST;                    // Generate SOCB on 1st event
    //
    // setup EPWM1_ADCSOCB as ADCSOCTrigger4
    //
    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTR_PRD;               // Select SOCB on TBCTR = PRD Event
    EPwm1Regs.ETSEL.bit.SOCBEN  = 1;                        // Enable SOCB
    EPwm1Regs.ETPS.bit.SOCBPRD  = ET_1ST;                   // Generate SOCB on 1st event

#elif CONVERTER_TYPE == THREE_PHASE
    //
    // setup EPWM4_ADCSOCA as ADCSOCTrigger1
    //
    EPwm4Regs.CMPB.bit.CMPB     = (Uint16)(Acqps*0.5f);     // CMPB = ACQPS/2
    EPwm4Regs.ETSEL.bit.SOCASEL = ET_CTRD_CMPB;             // Select SOCA on TBCTR_D = CMPB Event
    EPwm4Regs.ETSEL.bit.SOCAEN  = 1;                        // Enable SOCA
    EPwm4Regs.ETPS.bit.SOCAPRD  = ET_1ST;                   // Generate SOCA on 1st event
    //
    // setup EPWM4_ADCSOCA as ADCSOCTrigger2
    //
    EPwm5Regs.CMPB.bit.CMPB     = (Uint16)(TimeBase/4);  // CMPB = TimeBase/4
    EPwm5Regs.ETSEL.bit.SOCASEL = ET_CTRD_CMPB;              // Select SOCA on TBCTR_U = CMPB Event
    EPwm5Regs.ETSEL.bit.SOCAEN  = 1;                         // Enable SOCA
    EPwm5Regs.ETPS.bit.SOCAPRD  = ET_1ST;                    // Generate SOCA on 1st event
    //
    // setup EPWM4_ADCSOCB as ADCSOCTrigger3
    //
    EPwm5Regs.ETSEL.bit.SOCBSEL = ET_CTRU_CMPB;              // Select SOCB on TBCTR_D = CMPB Event
    EPwm5Regs.ETSEL.bit.SOCBEN  = 1;                         // Enable SOCB
    EPwm5Regs.ETPS.bit.SOCBPRD  = ET_1ST;                    // Generate SOCB on 1st event
    //
    // setup EPWM3_ADCSOCB as ADCSOCTrigger4
    //
    EPwm4Regs.ETSEL.bit.SOCBSEL = ET_CTR_PRD;               // Select SOCB on TBCTR = PRD Event
    EPwm4Regs.ETSEL.bit.SOCBEN  = 1;                        // Enable SOCB
    EPwm4Regs.ETPS.bit.SOCBPRD  = ET_1ST;                   // Generate SOCB on 1st event
#else
#endif
}

//
//interrupt setup
//
void NPC_HAL_enableEPWMInterruptGeneration(void)
{
    //
    // setup EPWM1_Interrupt
    //
#if CONVERTER_TYPE == SINGLE_PHASE
    EPwm1Regs.ETSEL.bit.INTSEL    = ET_CTR_ZERO;          // Select Interrupt on TBCTR_zero
    EPwm1Regs.ETSEL.bit.INTEN     = 1;                    // Enable Interrupt
    EPwm1Regs.ETPS.bit.INTPRD     = ET_1ST;               // Generate Interrupt on 1st event
    EPwm1Regs.ETCLR.bit.INT       = 1;                    // clear INT1 flag

    EPwm4Regs.ETSEL.bit.INTSEL    = ET_CTR_ZERO;          // Select Interrupt on TBCTR_zero
    EPwm4Regs.ETSEL.bit.INTEN     = 1;                    // Enable INT
    EPwm4Regs.ETPS.bit.INTPRD     = ET_1ST;               // Generate INT on 1st event
    EPwm4Regs.ETCLR.bit.INT       = 1;                    // clear INT1 flag

#elif CONVERTER_TYPE == THREE_PHASE
    EPwm4Regs.ETSEL.bit.INTSEL    = ET_CTR_ZERO;         // Select Interrupt on TBCTR_U_CMPC
    EPwm4Regs.ETSEL.bit.INTEN     = 1;                   // Enable Interrupt
    EPwm4Regs.ETPS.bit.INTPRD     = ET_1ST;              // Generate Interrupt on 1st event
    EPwm4Regs.ETCLR.bit.INT       = 1;                   //clear INT1 flag
#else
#endif
}

void NPC_HAL_setup_AUX_ISR_interrupt(void)
{
    CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    CPUTimer_clearOverflowFlag(CPUTIMER2_BASE);
}

//
//ePWM clock
//
void NPC_HAL_disablePWMCLKCounting(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;
}
void NPC_HAL_enablePWMCLKCounting(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

//
// epwm as GPIO initialization
//
void NPC_HAL_setGpioPinsAsPWM(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE

    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();
    InitEPwm8Gpio();

#elif CONVERTER_TYPE == THREE_PHASE

    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();
    InitEPwm5Gpio();
    InitEPwm6Gpio();
    InitEPwm8Gpio();
#else
#endif
}



//
//Set up peripherals for Board protection/Over current protection
//

/**************Sub part1**************/
//
//Set up CMPSS peripherals
//
static inline void NPC_HAL_setupCMPSS(uint32_t base1,
                     float32_t current_limit,
                     float32_t current_max_sense)
{
    //
    // Enable CMPSS1
    //
    CMPSS_enableModule(base1);

    //
    // Use VDDA as the reference for comparator DACs
    //
    CMPSS_configDAC(base1,
                    CMPSS_DACVAL_SYSCLK |
                    CMPSS_DACREF_VDDA |
                    CMPSS_DACSRC_SHDW);

    //
    // Set DAC to H~75% and L ~25% values
    //The UpperTrip_Limit = Voffset_counts + ((current_limit/currentmax_sense)*Voffset_counts)
    //The LowerTrip_Limit = Voffset_counts - ((current_limit/currentmax_sense)*Voffset_counts)
    //Voffset = 1.65v --> 2048counts & currentmax_sense(A) = Voffset/gain
    //
#if CONVERTER_TYPE == THREE_PHASE
    CMPSS_setDACValueHigh(base1, Iconv_sense_offset + (int16_t)((float32_t)current_limit *
                          (float32_t)(4095.0f - Iconv_sense_offset) / (float32_t)current_max_sense));
    CMPSS_setDACValueLow(base1, Iconv_sense_offset - (int16_t)((float32_t)current_limit *
                         (float32_t)(4095.0f - Iconv_sense_offset) / (float32_t)current_max_sense));
#elif CONVERTER_TYPE == SINGLE_PHASE
    CMPSS_setDACValueHigh(base1, Iconv_sense_offset + (int16_t)((float32_t)current_limit *
                          (float32_t)Iconv_sense_offset / (float32_t)current_max_sense));
    CMPSS_setDACValueLow(base1, Iconv_sense_offset - (int16_t)((float32_t)current_limit *
                         (float32_t)Iconv_sense_offset / (float32_t)current_max_sense));
#endif
    //
    // Make sure the asynchronous path compare high and low event
    // does not go to the OR gate with latched digital filter output
    // hence no additional parameter CMPSS_OR_ASYNC_OUT_W_FILT  is passed
    // comparator output is "not" inverted for high compare event
    //
    CMPSS_configHighComparator(base1, CMPSS_INSRC_DAC);

    //
    // Comparator output is inverted for for low compare event
    //
    CMPSS_configLowComparator(base1, CMPSS_INSRC_DAC | CMPSS_INV_INVERTED);

    CMPSS_configFilterHigh(base1, 2, 30, 20);
    CMPSS_configFilterLow(base1, 2, 30, 20);

    //
    // Reset filter logic & start filtering
    //
    CMPSS_initFilterHigh(base1);
    CMPSS_initFilterLow(base1);

    //
    // Configure CTRIPOUT path
    //
    CMPSS_configOutputsHigh(base1, CMPSS_TRIP_FILTER | CMPSS_TRIP_FILTER);
    CMPSS_configOutputsLow(base1, CMPSS_TRIP_FILTER | CMPSS_TRIP_FILTER);

    //
    // Comparator hysteresis control , set to 2x typical value
    //
    CMPSS_setHysteresis(base1, 2);

    //
    // Clear the latched comparator events
    //
    CMPSS_clearFilterLatchHigh(base1);
    CMPSS_clearFilterLatchLow(base1);
}
/**************Sub part2**************/
//
//Set up ePWM Trip Zone Module
//
static inline void NPC_HAL_setupPWMforTrip(uint32_t base)
{
    //
    // Trip 4 is the input to the DCAHCOMPSEL
    //
    EPWM_selectDigitalCompareTripInput(base,
                                      EPWM_DC_TRIP_TRIPIN4,
                                      EPWM_DC_TYPE_DCAH);                   // Trip source for DCAH is selected as TRIPIN4
    EPWM_setTripZoneDigitalCompareEventCondition(base,
                                                EPWM_TZ_DC_OUTPUT_A1,
                                                EPWM_TZ_EVENT_DCXH_HIGH);   // DC block output(DC_EVENT) is chosen as DC_OUTPUT_A1,
                                                                            // which has a trip event when DCAH is High
    EPWM_setDigitalCompareEventSource(base,
                                     EPWM_DC_MODULE_A,
                                     EPWM_DC_EVENT_1,
                                     EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);     // source for DCAEVENT_1 is chosen as DC_OUTPUT_A1 & filter is bypassed
    EPWM_setDigitalCompareEventSyncMode(base,
                                       EPWM_DC_MODULE_A,
                                       EPWM_DC_EVENT_1,
                                       EPWM_DC_EVENT_INPUT_NOT_SYNCED);     // DCAEVENT_1 is chosen as ASYNC input to EPWM TripZone module

    //
    // Enable the following trips - DCAEVT1
    //
    EPWM_enableTripZoneSignals(base, EPWM_TZ_SIGNAL_DCAEVT1);               // TZ signal for TripZone module is chosen as DCAEVT1,
                                                                            // which will be set high when the DCAH is High

    //
    // What do we want the OST / CBC events to do?
    // TZA events can force EPWMxA
    // TZB events can force EPWMxB
    //
    EPWM_setTripZoneAction(base, EPWM_TZ_ACTION_EVENT_TZA, EPWM_TZ_ACTION_LOW);
    EPWM_setTripZoneAction(base, EPWM_TZ_ACTION_EVENT_TZB, EPWM_TZ_ACTION_LOW);
    //
    //Note: The source for TZA events are TZ1 - TZ6, DCAEVT1/2, DCBEVT1/2 & since 'EPWM_TZ_ACTION_EVENT_TZA' has higher priority
    // over 'EPWM_TZ_ACTION_EVENT_DCAEVT1', TZA is chosen tzEvent to determine the actions on EPWMxA. similarly for TZB too.

    //
    // Clear any spurious DCAEVT1 flags
    //
    EPWM_clearTripZoneFlag(base, EPWM_TZ_FLAG_DCAEVT1);

    //
    // Force a trip on PWM to safely start the system
    //
    EPWM_forceTripZoneEvent(base, EPWM_TZ_FORCE_EVENT_OST);

}

void NPC_HAL_setupBoardProtection(void)
{
    //
    // Disable all the muxes first
    //
    XBAR_disableEPWMMux(XBAR_TRIP4, 0xFF);
#if BOARD_PROTECTION_CMPSS == 1

#if CONVERTER_TYPE == SINGLE_PHASE

/***************SetUp OverCurrent Protection On Phase-A**********************/
    NPC_HAL_setupCMPSS(NPC_A_CMPSS_BASE,
                       Iconv_TRIP_LIMIT_AMPS,
                       Iconv_MAX_SENSE_AMPS);

    XBAR_setEPWMMuxConfig(XBAR_TRIP4, NPC_A_XBAR_MUX_VAL);

    XBAR_enableEPWMMux(XBAR_TRIP4, NPC_A_XBAR_MUX);

    XBAR_clearInputFlag(NPC_A_XBAR_FLAG1);
    XBAR_clearInputFlag(NPC_A_XBAR_FLAG2);

/***************SetUp ePWM TZ module in OST mode for any of the above CMPSS events**********************/
    NPC_HAL_setupPWMforTrip(EPWM1_BASE);
    NPC_HAL_setupPWMforTrip(EPWM2_BASE);

#elif CONVERTER_TYPE == THREE_PHASE

/***************SetUp OverCurrent Protection On Phase-A**********************/
    NPC_HAL_setupCMPSS(NPC_A_CMPSS_BASE,
                       Iconv_TRIP_LIMIT_AMPS,
                       Iconv_MAX_SENSE_AMPS);

    XBAR_setEPWMMuxConfig(XBAR_TRIP4, NPC_A_XBAR_MUX_VAL);

    XBAR_enableEPWMMux(XBAR_TRIP4, NPC_A_XBAR_MUX);

    XBAR_clearInputFlag(NPC_A_XBAR_FLAG1);
    XBAR_clearInputFlag(NPC_A_XBAR_FLAG2);

/***************SetUp OverCurrent Protection On Phase-B**********************/
    NPC_HAL_setupCMPSS(NPC_B_CMPSS_BASE,
                       Iconv_TRIP_LIMIT_AMPS,
                        Iconv_MAX_SENSE_AMPS);

    XBAR_setEPWMMuxConfig(XBAR_TRIP4, NPC_B_XBAR_MUX_VAL);

    XBAR_enableEPWMMux(XBAR_TRIP4, NPC_B_XBAR_MUX);

    XBAR_clearInputFlag(NPC_B_XBAR_FLAG1);
    XBAR_clearInputFlag(NPC_B_XBAR_FLAG2);

/***************SetUp OverCurrent Protection On Phase-C**********************/
    NPC_HAL_setupCMPSS(NPC_C_CMPSS_BASE,
                       Iconv_TRIP_LIMIT_AMPS,
                        Iconv_MAX_SENSE_AMPS);

    XBAR_setEPWMMuxConfig(XBAR_TRIP4, NPC_C_XBAR_MUX_VAL);

    XBAR_enableEPWMMux(XBAR_TRIP4, NPC_C_XBAR_MUX);

    XBAR_clearInputFlag(NPC_C_XBAR_FLAG1);
    XBAR_clearInputFlag(NPC_C_XBAR_FLAG2);

/***************SetUp ePWM TZ module in OST mode for any of the above CMPSS events**********************/
    NPC_HAL_setupPWMforTrip(EPWM4_BASE);
    NPC_HAL_setupPWMforTrip(EPWM5_BASE);
    NPC_HAL_setupPWMforTrip(EPWM6_BASE);
    NPC_HAL_setupPWMforTrip(EPWM3_BASE);
    NPC_HAL_setupPWMforTrip(EPWM1_BASE);
    NPC_HAL_setupPWMforTrip(EPWM2_BASE);

#else
#endif

#else
#endif
/*******************Fault pin based protection*********************/
#if BOARD_PROTECTION_DESAT == 1
    GPIO_setDirectionMode(NPC_FAULT_GPIO, GPIO_DIR_MODE_IN);
    GPIO_setQualificationMode(NPC_FAULT_GPIO, GPIO_QUAL_6SAMPLE);
    GPIO_setPinConfig(NPC_FAULT_GPIO_PIN_CONFIG);
    GPIO_setPadConfig(NPC_FAULT_GPIO, GPIO_PIN_TYPE_INVERT
                          | GPIO_PIN_TYPE_STD );

    XBAR_setInputPin(XBAR_INPUT1, NPC_FAULT_GPIO);

    //XBAR_setEPWMMuxConfig(XBAR_TRIP4, NPC_FAULT_XBAR_MUX_VAL);

    //XBAR_enableEPWMMux(XBAR_TRIP4, NPC_FAULT_XBAR_MUX);

    XBAR_clearInputFlag(NPC_FAULT_XBAR_FLAG);

/***************SetUp ePWM TZ module in OST mode for any of the above CMPSS events**********************/
    NPC_HAL_setupPWMforTrip(EPWM4_BASE);
    NPC_HAL_setupPWMforTrip(EPWM5_BASE);
    NPC_HAL_setupPWMforTrip(EPWM6_BASE);
    NPC_HAL_setupPWMforTrip(EPWM3_BASE);
    NPC_HAL_setupPWMforTrip(EPWM1_BASE);
    NPC_HAL_setupPWMforTrip(EPWM2_BASE);
#else
#endif

}


#if CONVERTER_TYPE == THREE_PHASE

void NPC_HAL_setupGateDriverDESATGPIO(void)
{
    GPIO_setDirectionMode(NPC_FAULT_GPIO, GPIO_DIR_MODE_IN);
    GPIO_setPinConfig(NPC_FAULT_GPIO_PIN_CONFIG);
}

bool NPC_HAL_getGateDriverDESATGPIOStatus(void)
{
    return GPIO_readPin(NPC_FAULT_GPIO);
}

#else
#endif
//
// INV-ON/OFF pin related
//
void NPC_HAL_setup_INVONOFF_GPIO(void)
{
    GPIO_setDirectionMode(NPC_INVONOFF_GPIO, GPIO_DIR_MODE_IN);
    GPIO_setPinConfig(NPC_INVONOFF_GPIO_PIN_CONFIG);
}

bool NPC_HAL_get_INVONOFF_GPIOStatus(void)
{
    return GPIO_readPin(NPC_INVONOFF_GPIO);
}



//
//Initialize LED
//
void NPC_HAL_setupLEDGPIO(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE

    //
    // Enable an BLUE LED (GPIO31)
    //
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO31      = 1;     // Disable pullup on GPIO31
    GpioCtrlRegs.GPAMUX2.bit.GPIO31     = 0;     // GPIO31 = GPIO31
    GpioCtrlRegs.GPAQSEL2.bit.GPIO31    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPASET.bit.GPIO31      = 1;     // Load output latch(set the GPIO)
    GpioCtrlRegs.GPADIR.bit.GPIO31      = 1;     // GPIO31 = output
    EDIS;

    //
    // Enable an RED LED (GPIO34)
    //
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO34      = 1;     // Disable pullup on GPIO34
    GpioCtrlRegs.GPBMUX1.bit.GPIO34     = 0;     // GPIO34 = GPIO34
    GpioCtrlRegs.GPBQSEL1.bit.GPIO34    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPBSET.bit.GPIO34      = 1;     // Load output latch(set the GPIO)
    GpioCtrlRegs.GPBDIR.bit.GPIO34      = 1;     // GPIO34 = output
    EDIS;


#elif CONVERTER_TYPE == THREE_PHASE

    //
    // Enable an BLUE LED (GPIO31)
    //
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO31      = 1;     // Disable pullup on GPIO31
    GpioCtrlRegs.GPAMUX2.bit.GPIO31     = 0;     // GPIO31 = GPIO31
    GpioCtrlRegs.GPAQSEL2.bit.GPIO31    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPASET.bit.GPIO31      = 1;     // Load output latch(set the GPIO)
    GpioCtrlRegs.GPADIR.bit.GPIO31      = 1;     // GPIO31 = output
    EDIS;

    //
    // Enable an RED LED (GPIO34)
    //
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO34      = 1;     // Disable pullup on GPIO34
    GpioCtrlRegs.GPBMUX1.bit.GPIO34     = 0;     // GPIO34 = GPIO34
    GpioCtrlRegs.GPBQSEL1.bit.GPIO34    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPBSET.bit.GPIO34      = 1;     // Load output latch(set the GPIO)
    GpioCtrlRegs.GPBDIR.bit.GPIO34      = 1;     // GPIO34 = output
    EDIS;

    //
    // AC_OV_LED (GPIO139)
    //
    EALLOW;
    GpioCtrlRegs.GPEPUD.bit.GPIO139      = 1;     // Disable pullup on GPIO139
    GpioCtrlRegs.GPEMUX1.bit.GPIO139     = 0;     // GPIO139 = GPIO139
    GpioCtrlRegs.GPEQSEL1.bit.GPIO139    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPECLEAR.bit.GPIO139    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPEDIR.bit.GPIO139      = 1;     // GPIO139 = output
    EDIS;

    //
    // AC_UV_LED (GPIO97)
    //
    EALLOW;
    GpioCtrlRegs.GPDPUD.bit.GPIO97      = 1;     // Disable pullup on GPIO97
    GpioCtrlRegs.GPDMUX1.bit.GPIO97     = 0;     // GPIO97 = GPIO97
    GpioCtrlRegs.GPDQSEL1.bit.GPIO97    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPDCLEAR.bit.GPIO97    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPDDIR.bit.GPIO97      = 1;     // GPIO97 = output
    EDIS;

    //
    // AC_OC_LED (GPIO52)
    //
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO52      = 1;     // Disable pullup on GPIO52
    GpioCtrlRegs.GPBMUX2.bit.GPIO52     = 0;     // GPIO52 = GPIO52
    GpioCtrlRegs.GPBQSEL2.bit.GPIO52    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPBCLEAR.bit.GPIO52    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPBDIR.bit.GPIO52      = 1;     // GPIO52 = output
    EDIS;

    //
    // DC_OV_LED (GPIO94)
    //
    EALLOW;
    GpioCtrlRegs.GPCPUD.bit.GPIO94      = 1;     // Disable pullup on GPIO94
    GpioCtrlRegs.GPCMUX2.bit.GPIO94     = 0;     // GPIO94 = GPIO94
    GpioCtrlRegs.GPCQSEL2.bit.GPIO94    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPCCLEAR.bit.GPIO94    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPCDIR.bit.GPIO94      = 1;     // GPIO94 = output
    EDIS;

    //
    // DESAT_LED (GPIO41)
    //
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO41      = 1;     // Disable pullup on GPIO41
    GpioCtrlRegs.GPBMUX1.bit.GPIO41     = 0;     // GPIO41 = GPIO41
    GpioCtrlRegs.GPBQSEL1.bit.GPIO41    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPBCLEAR.bit.GPIO41    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPBDIR.bit.GPIO41      = 1;     // GPIO41 = output
    EDIS;

    //
    // PFC_RDY_LED (GPIO40)
    //
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO40      = 1;     // Disable pullup on GPIO40
    GpioCtrlRegs.GPBMUX1.bit.GPIO40     = 0;     // GPIO40 = GPIO40
    GpioCtrlRegs.GPBQSEL1.bit.GPIO40    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPBCLEAR.bit.GPIO40    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPBDIR.bit.GPIO40      = 1;     // GPIO40 = output
    EDIS;

    //
    // LED1 (GPIO60)
    //
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO60      = 1;     // Disable pullup on GPIO60
    GpioCtrlRegs.GPBMUX2.bit.GPIO60     = 0;     // GPIO60 = GPIO60
    GpioCtrlRegs.GPBQSEL2.bit.GPIO60    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPBCLEAR.bit.GPIO60    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPBDIR.bit.GPIO60      = 1;     // GPIO60 = output
    EDIS;

    //
    // LED2 (GPIO22)
    //
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO22      = 1;     // Disable pullup on GPIO22
    GpioCtrlRegs.GPAMUX2.bit.GPIO22     = 0;     // GPIO22 = GPIO22
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22    = 3;     // Select Qualification as Asynchronous
    GpioDataRegs.GPACLEAR.bit.GPIO22    = 1;     // Load output latch(clear the GPIO)
    GpioCtrlRegs.GPADIR.bit.GPIO22      = 1;     // GPIO22 = output
    EDIS;

#else
#endif

}

void NPC_HAL_TurnON_REDLED(void)
{
    EALLOW;
    GpioDataRegs.GPBCLEAR.bit.GPIO34      = 1;  // Load output latch(Clear the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_BLUELED(void)
{
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO31      = 1;  // Load output latch(Clear the GPIO)
    EDIS;
}

#if CONVERTER_TYPE == THREE_PHASE

void NPC_HAL_TurnON_AC_OV_LED(void)
{
    EALLOW;
    GpioDataRegs.GPESET.bit.GPIO139    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_AC_UV_LED(void)
{
    EALLOW;
    GpioDataRegs.GPDSET.bit.GPIO97    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_AC_OC_LED(void)
{
    EALLOW;
    GpioDataRegs.GPBSET.bit.GPIO52    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_DC_OV_LED(void)
{
    EALLOW;
    GpioDataRegs.GPCSET.bit.GPIO94    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_DESAT_LED(void)
{
    EALLOW;
    GpioDataRegs.GPBSET.bit.GPIO41    = 1;     // Load output NPC_HAL_TurnOFF_LED1latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_PFC_RDY_LED(void)
{
    EALLOW;
    GpioDataRegs.GPBSET.bit.GPIO40    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnOFF_PFC_RDY_LED(void)
{
    EALLOW;
    GpioDataRegs.GPBCLEAR.bit.GPIO40    = 1;     // Load output latch(Clear the GPIO)
    EDIS;
}
void NPC_HAL_TurnOFF_AC_OV_LED(void)
{
    EALLOW;
    GpioDataRegs.GPECLEAR.bit.GPIO139    = 1;     // Load output latch(clear the GPIO)
    EDIS;
}
void NPC_HAL_TurnOFF_AC_UV_LED(void)
{
    EALLOW;
    GpioDataRegs.GPDCLEAR.bit.GPIO97    = 1;     // Load output latch(clear the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_LED1(void)
{
    EALLOW;
    GpioDataRegs.GPBSET.bit.GPIO60    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnOFF_LED1(void)
{
    EALLOW;
    GpioDataRegs.GPBCLEAR.bit.GPIO60    = 1;     // Load output latch(clear the GPIO)
    EDIS;
}
void NPC_HAL_TurnON_LED2(void)
{
    EALLOW;
    GpioDataRegs.GPASET.bit.GPIO22    = 1;     // Load output latch(Set the GPIO)
    EDIS;
}
void NPC_HAL_TurnOFF_LED2(void)
{
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO22    = 1;     // Load output latch(clear the GPIO)
    EDIS;
}

#else
#endif

//
//Initialize RelayGPIO
//
void NPC_HAL_setupRelayGPIO(void)
{
    //
    // Relay_GPIO (GPIO67)
    //
    EALLOW;
    GpioCtrlRegs.GPCPUD.bit.GPIO67      = 1;     // Disable pullup on GPIO67
    GpioCtrlRegs.GPCMUX1.bit.GPIO67     = 0;     // GPIO67 = GPIO67
    GpioCtrlRegs.GPCQSEL1.bit.GPIO67    = 3;     // Select Qualification as Asynchronous
    GpioCtrlRegs.GPCDIR.bit.GPIO67      = 1;     // GPIO67 = output
    GpioDataRegs.GPCCLEAR.bit.GPIO67    = 1;
    EDIS;
}

void NPC_HAL_TurnONRelay(void)
{
    EALLOW;
    GpioDataRegs.GPCSET.bit.GPIO67    = 1;
    EDIS;
}


//
//*******  Configure SCI & SCI Functions**********//
//

void NPC_HAL_SCI_B_GPIO_18_19_Init(void)
{
    GPIO_setPinConfig(GPIO_19_SCIB_RX);
    GPIO_setPinConfig(GPIO_18_SCIB_TX);
}

void NPC_HAL_SCI_C_GPIO_56_139_Init(void)
{
    GPIO_setPinConfig(GPIO_56_SCIC_TX);
    GPIO_setPinConfig(GPIO_139_SCIC_RX);
}

void NPC_HAL_config_SCI_polling(uint32_t base)
{
    SCI_performSoftwareReset(base);
    SCI_setConfig(base, 25000000, 9600, (SCI_CONFIG_WLEN_8 |
                                                        SCI_CONFIG_STOP_ONE |
                                                        SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(base);
    SCI_resetRxFIFO(base);
    SCI_resetTxFIFO(base);
    SCI_clearInterruptStatus(base, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(base);
    SCI_enableModule(base);
    SCI_performSoftwareReset(base);
}

void NPC_HAL_config_SCI_interrupt(uint32_t base)
{
    SCI_performSoftwareReset(base);
    SCI_setConfig(base, 25000000, 9600, (SCI_CONFIG_WLEN_8 |
                                                        SCI_CONFIG_STOP_ONE |
                                                        SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(base);
    SCI_resetRxFIFO(base);
    SCI_resetTxFIFO(base);
    SCI_clearInterruptStatus(base, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(base);
    SCI_enableInterrupt(base, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_setFIFOInterruptLevel(base, SCI_FIFO_TX1, SCI_FIFO_RX1);
    SCI_enableModule(base);
    SCI_performSoftwareReset(base);
}



