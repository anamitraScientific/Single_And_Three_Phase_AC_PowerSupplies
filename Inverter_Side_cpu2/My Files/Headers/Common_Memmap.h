/*
 * Common_Memmap.h
 *
 *  Created on: Jul 22, 2025
 *      Author: Anamitra Sarkar
 */

#ifndef COMMON_MEMMAP_H_
#define COMMON_MEMMAP_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "device.h"
#include "driverlib.h"
#include "Config.h"
#include "Control_Variables.h"

typedef enum{
    TYPE_FLOAT32,
    TYPE_INT32
} DataType;

typedef struct{
    uint16_t BUF_ADR;
    volatile void *variable_ptr;
    DataType type;
} DataMapEntry;

#if CONVERTER_TYPE == SINGLE_PHASE

#define    BUFF_ADR_SET_VAC                          1
#define    BUFF_ADR_SET_VDC                          BUFF_ADR_SET_VAC + 4
#define    BUFF_ADR_SET_FREQ                         BUFF_ADR_SET_VDC + 4
#define    BUFF_ADR_VAC_SLEW                         BUFF_ADR_SET_FREQ + 4
#define    BUFF_ADR_VDC_SLEW                         BUFF_ADR_VAC_SLEW + 4
#define    BUFF_ADR_FREQ_SLEW                        BUFF_ADR_VDC_SLEW + 4
#define    BUFF_ADR_OUT_STATE                        BUFF_ADR_FREQ_SLEW + 4
#define    BUFF_ADR_ON_DEGREE                        BUFF_ADR_OUT_STATE + 1
#define    BUFF_ADR_OFF_DEGREE                       BUFF_ADR_ON_DEGREE + 4
#define    BUFF_ADR_COUPLE                           BUFF_ADR_OFF_DEGREE + 4

#define    BUFF_ADR_LIM_VAC                         (BUFF_ADR_COUPLE + 1)
#define    BUFF_ADR_LIM_VDC_P                       (BUFF_ADR_LIM_VAC + 4)
#define    BUFF_ADR_LIM_VDC_M                       (BUFF_ADR_LIM_VDC_P + 4)
#define    BUFF_ADR_LIM_FREQ                        (BUFF_ADR_LIM_VDC_M + 4)
#define    BUFF_ADR_LIM_OPP                         (BUFF_ADR_LIM_FREQ + 4)
#define    BUFF_ADR_LIM_OCP                         (BUFF_ADR_LIM_OPP + 4)
#define    BUFF_ADR_LIM_OCP_DLY                     (BUFF_ADR_LIM_OCP + 4)

#define    BUFF_ADR_HARM_WAVEFORM_NUM               (BUFF_ADR_LIM_OCP_DLY + 4)              //63

#define    BUFF_ADR_HARM1_NO                        (BUFF_ADR_HARM_WAVEFORM_NUM + 1)        //64
#define    BUFF_ADR_HARM1_AMP                       (BUFF_ADR_HARM1_NO + 4)                 //68
#define    BUFF_ADR_HARM1_PHASE                     (BUFF_ADR_HARM1_AMP + 4)                //72

#define    BUFF_ADR_HARM_BASE                       (BUFF_ADR_HARM1_NO)
#define    BUFF_ADR_HARM_NUM(n)                     (BUFF_ADR_HARM_BASE + ((n-1) * 12))
#define    BUFF_ADR_HARM_AMP(n)                     (BUFF_ADR_HARM_BASE + ((n-1) * 12) + 4)
#define    BUFF_ADR_HARM_PHASE(n)                   (BUFF_ADR_HARM_BASE + ((n-1) * 12) + 8)

#define    BUFF_ADR_HARM2_NO                        (BUFF_ADR_HARM1_PHASE + 4)              //76
#define    BUFF_ADR_HARM2_AMP                       (BUFF_ADR_HARM2_NO + 4)                 //80
#define    BUFF_ADR_HARM2_PHASE                     (BUFF_ADR_HARM2_AMP + 4)

#define    BUFF_ADR_HARM3_NO                        (BUFF_ADR_HARM2_PHASE + 4)
#define    BUFF_ADR_HARM3_AMP                       (BUFF_ADR_HARM3_NO + 4)
#define    BUFF_ADR_HARM3_PHASE                     (BUFF_ADR_HARM3_AMP + 4)

#define    BUFF_ADR_HARM4_NO                        (BUFF_ADR_HARM3_PHASE + 4)
#define    BUFF_ADR_HARM4_AMP                       (BUFF_ADR_HARM4_NO + 4)
#define    BUFF_ADR_HARM4_PHASE                     (BUFF_ADR_HARM4_AMP + 4)

#define    BUFF_ADR_HARM5_NO                        (BUFF_ADR_HARM4_PHASE + 4)
#define    BUFF_ADR_HARM5_AMP                       (BUFF_ADR_HARM5_NO + 4)
#define    BUFF_ADR_HARM5_PHASE                     (BUFF_ADR_HARM5_AMP + 4)

#define    BUFF_ADR_HARM6_NO                        (BUFF_ADR_HARM5_PHASE + 4)
#define    BUFF_ADR_HARM6_AMP                       (BUFF_ADR_HARM6_NO + 4)
#define    BUFF_ADR_HARM6_PHASE                     (BUFF_ADR_HARM6_AMP + 4)

#define    BUFF_ADR_HARM7_NO                        (BUFF_ADR_HARM6_PHASE + 4)
#define    BUFF_ADR_HARM7_AMP                       (BUFF_ADR_HARM7_NO + 4)
#define    BUFF_ADR_HARM7_PHASE                     (BUFF_ADR_HARM7_AMP + 4)

#define    BUFF_ADR_HARM8_NO                        (BUFF_ADR_HARM7_PHASE + 4)
#define    BUFF_ADR_HARM8_AMP                       (BUFF_ADR_HARM8_NO + 4)
#define    BUFF_ADR_HARM8_PHASE                     (BUFF_ADR_HARM8_AMP + 4)

#define    BUFF_ADR_HARM9_NO                        (BUFF_ADR_HARM8_PHASE + 4)
#define    BUFF_ADR_HARM9_AMP                       (BUFF_ADR_HARM9_NO + 4)
#define    BUFF_ADR_HARM9_PHASE                     (BUFF_ADR_HARM9_AMP + 4)

#define    BUFF_ADR_HARM10_NO                       (BUFF_ADR_HARM9_PHASE + 4)
#define    BUFF_ADR_HARM10_AMP                      (BUFF_ADR_HARM10_NO + 4)
#define    BUFF_ADR_HARM10_PHASE                    (BUFF_ADR_HARM10_AMP + 4)

#define    BUFF_ADR_HARM11_NO                       (BUFF_ADR_HARM10_PHASE + 4)
#define    BUFF_ADR_HARM11_AMP                      (BUFF_ADR_HARM11_NO + 4)
#define    BUFF_ADR_HARM11_PHASE                    (BUFF_ADR_HARM11_AMP + 4)

#define    BUFF_ADR_HARM12_NO                       (BUFF_ADR_HARM11_PHASE + 4)
#define    BUFF_ADR_HARM12_AMP                      (BUFF_ADR_HARM12_NO + 4)
#define    BUFF_ADR_HARM12_PHASE                    (BUFF_ADR_HARM12_AMP + 4)

#define    BUFF_ADR_HARM13_NO                       (BUFF_ADR_HARM12_PHASE + 4)
#define    BUFF_ADR_HARM13_AMP                      (BUFF_ADR_HARM13_NO + 4)
#define    BUFF_ADR_HARM13_PHASE                    (BUFF_ADR_HARM13_AMP + 4)

#define    BUFF_ADR_HARM14_NO                       (BUFF_ADR_HARM13_PHASE + 4)
#define    BUFF_ADR_HARM14_AMP                      (BUFF_ADR_HARM14_NO + 4)
#define    BUFF_ADR_HARM14_PHASE                    (BUFF_ADR_HARM14_AMP + 4)

#define    BUFF_ADR_HARM15_NO                       (BUFF_ADR_HARM14_PHASE + 4)
#define    BUFF_ADR_HARM15_AMP                      (BUFF_ADR_HARM15_NO + 4)
#define    BUFF_ADR_HARM15_PHASE                    (BUFF_ADR_HARM15_AMP + 4)

#define    BUFF_ADR_HARM16_NO                       (BUFF_ADR_HARM15_PHASE + 4)
#define    BUFF_ADR_HARM16_AMP                      (BUFF_ADR_HARM16_NO + 4)
#define    BUFF_ADR_HARM16_PHASE                    (BUFF_ADR_HARM16_AMP + 4)

#define    BUFF_ADR_HARM17_NO                       (BUFF_ADR_HARM16_PHASE + 4)
#define    BUFF_ADR_HARM17_AMP                      (BUFF_ADR_HARM17_NO + 4)
#define    BUFF_ADR_HARM17_PHASE                    (BUFF_ADR_HARM17_AMP + 4)

#define    BUFF_ADR_HARM18_NO                       (BUFF_ADR_HARM17_PHASE + 4)
#define    BUFF_ADR_HARM18_AMP                      (BUFF_ADR_HARM18_NO + 4)
#define    BUFF_ADR_HARM18_PHASE                    (BUFF_ADR_HARM18_AMP + 4)

#define    BUFF_ADR_HARM19_NO                       (BUFF_ADR_HARM18_PHASE + 4)
#define    BUFF_ADR_HARM19_AMP                      (BUFF_ADR_HARM19_NO + 4)
#define    BUFF_ADR_HARM19_PHASE                    (BUFF_ADR_HARM19_AMP + 4)

#define    BUFF_ADR_HARM20_NO                       (BUFF_ADR_HARM19_PHASE + 4)
#define    BUFF_ADR_HARM20_AMP                      (BUFF_ADR_HARM20_NO + 4)
#define    BUFF_ADR_HARM20_PHASE                    (BUFF_ADR_HARM20_AMP + 4)

#define    BUFF_ADR_HARM21_NO                       (BUFF_ADR_HARM20_PHASE + 4)
#define    BUFF_ADR_HARM21_AMP                      (BUFF_ADR_HARM21_NO + 4)
#define    BUFF_ADR_HARM21_PHASE                    (BUFF_ADR_HARM21_AMP + 4)

#define    BUFF_ADR_HARM22_NO                       (BUFF_ADR_HARM21_PHASE + 4)
#define    BUFF_ADR_HARM22_AMP                      (BUFF_ADR_HARM22_NO + 4)
#define    BUFF_ADR_HARM22_PHASE                    (BUFF_ADR_HARM22_AMP + 4)

#define    BUFF_ADR_HARM23_NO                       (BUFF_ADR_HARM22_PHASE + 4)
#define    BUFF_ADR_HARM23_AMP                      (BUFF_ADR_HARM23_NO + 4)
#define    BUFF_ADR_HARM23_PHASE                    (BUFF_ADR_HARM23_AMP + 4)

#define    BUFF_ADR_HARM24_NO                       (BUFF_ADR_HARM23_PHASE + 4)
#define    BUFF_ADR_HARM24_AMP                      (BUFF_ADR_HARM24_NO + 4)
#define    BUFF_ADR_HARM24_PHASE                    (BUFF_ADR_HARM24_AMP + 4)

#define    BUFF_ADR_HARM25_NO                       (BUFF_ADR_HARM24_PHASE + 4)
#define    BUFF_ADR_HARM25_AMP                      (BUFF_ADR_HARM25_NO + 4)
#define    BUFF_ADR_HARM25_PHASE                    (BUFF_ADR_HARM25_AMP + 4)

#define    BUFF_ADR_HARM26_NO                       (BUFF_ADR_HARM25_PHASE + 4)
#define    BUFF_ADR_HARM26_AMP                      (BUFF_ADR_HARM26_NO + 4)
#define    BUFF_ADR_HARM26_PHASE                    (BUFF_ADR_HARM26_AMP + 4)

#define    BUFF_ADR_HARM27_NO                       (BUFF_ADR_HARM26_PHASE + 4)
#define    BUFF_ADR_HARM27_AMP                      (BUFF_ADR_HARM27_NO + 4)
#define    BUFF_ADR_HARM27_PHASE                    (BUFF_ADR_HARM27_AMP + 4)

#define    BUFF_ADR_HARM28_NO                       (BUFF_ADR_HARM27_PHASE + 4)
#define    BUFF_ADR_HARM28_AMP                      (BUFF_ADR_HARM28_NO + 4)
#define    BUFF_ADR_HARM28_PHASE                    (BUFF_ADR_HARM28_AMP + 4)

#define    BUFF_ADR_HARM29_NO                       (BUFF_ADR_HARM28_PHASE + 4)
#define    BUFF_ADR_HARM29_AMP                      (BUFF_ADR_HARM29_NO + 4)
#define    BUFF_ADR_HARM29_PHASE                    (BUFF_ADR_HARM29_AMP + 4)

#define    BUFF_ADR_HARM30_NO                       (BUFF_ADR_HARM29_PHASE + 4)
#define    BUFF_ADR_HARM30_AMP                      (BUFF_ADR_HARM30_NO + 4)
#define    BUFF_ADR_HARM30_PHASE                    (BUFF_ADR_HARM30_AMP + 4)

#define    BUFF_ADR_HARM31_NO                       (BUFF_ADR_HARM30_PHASE + 4)
#define    BUFF_ADR_HARM31_AMP                      (BUFF_ADR_HARM31_NO + 4)
#define    BUFF_ADR_HARM31_PHASE                    (BUFF_ADR_HARM31_AMP + 4)

#define    BUFF_ADR_HARM32_NO                       (BUFF_ADR_HARM31_PHASE + 4)
#define    BUFF_ADR_HARM32_AMP                      (BUFF_ADR_HARM32_NO + 4)
#define    BUFF_ADR_HARM32_PHASE                    (BUFF_ADR_HARM32_AMP + 4)

#define    BUFF_ADR_HARM33_NO                       (BUFF_ADR_HARM32_PHASE + 4)
#define    BUFF_ADR_HARM33_AMP                      (BUFF_ADR_HARM33_NO + 4)
#define    BUFF_ADR_HARM33_PHASE                    (BUFF_ADR_HARM33_AMP + 4)

#define    BUFF_ADR_HARM34_NO                       (BUFF_ADR_HARM33_PHASE + 4)
#define    BUFF_ADR_HARM34_AMP                      (BUFF_ADR_HARM34_NO + 4)
#define    BUFF_ADR_HARM34_PHASE                    (BUFF_ADR_HARM34_AMP + 4)

#define    BUFF_ADR_HARM35_NO                       (BUFF_ADR_HARM34_PHASE + 4)
#define    BUFF_ADR_HARM35_AMP                      (BUFF_ADR_HARM35_NO + 4)
#define    BUFF_ADR_HARM35_PHASE                    (BUFF_ADR_HARM35_AMP + 4)

#define    BUFF_ADR_HARM36_NO                       (BUFF_ADR_HARM35_PHASE + 4)
#define    BUFF_ADR_HARM36_AMP                      (BUFF_ADR_HARM36_NO + 4)
#define    BUFF_ADR_HARM36_PHASE                    (BUFF_ADR_HARM36_AMP + 4)

#define    BUFF_ADR_HARM37_NO                       (BUFF_ADR_HARM36_PHASE + 4)
#define    BUFF_ADR_HARM37_AMP                      (BUFF_ADR_HARM37_NO + 4)
#define    BUFF_ADR_HARM37_PHASE                    (BUFF_ADR_HARM37_AMP + 4)

#define    BUFF_ADR_HARM38_NO                       (BUFF_ADR_HARM37_PHASE + 4)
#define    BUFF_ADR_HARM38_AMP                      (BUFF_ADR_HARM38_NO + 4)
#define    BUFF_ADR_HARM38_PHASE                    (BUFF_ADR_HARM38_AMP + 4)

#define    BUFF_ADR_HARM39_NO                       (BUFF_ADR_HARM38_PHASE + 4)
#define    BUFF_ADR_HARM39_AMP                      (BUFF_ADR_HARM39_NO + 4)
#define    BUFF_ADR_HARM39_PHASE                    (BUFF_ADR_HARM39_AMP + 4)

#define    BUFF_ADR_HARM40_NO                       (BUFF_ADR_HARM39_PHASE + 4)
#define    BUFF_ADR_HARM40_AMP                      (BUFF_ADR_HARM40_NO + 4)
#define    BUFF_ADR_HARM40_PHASE                    (BUFF_ADR_HARM40_AMP + 4)

#define    BUFF_ADR_HARM41_NO                       (BUFF_ADR_HARM40_PHASE + 4)
#define    BUFF_ADR_HARM41_AMP                      (BUFF_ADR_HARM41_NO + 4)
#define    BUFF_ADR_HARM41_PHASE                    (BUFF_ADR_HARM41_AMP + 4)

#define    BUFF_ADR_HARM42_NO                       (BUFF_ADR_HARM41_PHASE + 4)
#define    BUFF_ADR_HARM42_AMP                      (BUFF_ADR_HARM42_NO + 4)
#define    BUFF_ADR_HARM42_PHASE                    (BUFF_ADR_HARM42_AMP + 4)

#define    BUFF_ADR_HARM43_NO                       (BUFF_ADR_HARM42_PHASE + 4)
#define    BUFF_ADR_HARM43_AMP                      (BUFF_ADR_HARM43_NO + 4)
#define    BUFF_ADR_HARM43_PHASE                    (BUFF_ADR_HARM43_AMP + 4)

#define    BUFF_ADR_HARM44_NO                       (BUFF_ADR_HARM43_PHASE + 4)
#define    BUFF_ADR_HARM44_AMP                      (BUFF_ADR_HARM44_NO + 4)
#define    BUFF_ADR_HARM44_PHASE                    (BUFF_ADR_HARM44_AMP + 4)

#define    BUFF_ADR_HARM45_NO                       (BUFF_ADR_HARM44_PHASE + 4)
#define    BUFF_ADR_HARM45_AMP                      (BUFF_ADR_HARM45_NO + 4)
#define    BUFF_ADR_HARM45_PHASE                    (BUFF_ADR_HARM45_AMP + 4)

#define    BUFF_ADR_HARM46_NO                       (BUFF_ADR_HARM45_PHASE + 4)
#define    BUFF_ADR_HARM46_AMP                      (BUFF_ADR_HARM46_NO + 4)
#define    BUFF_ADR_HARM46_PHASE                    (BUFF_ADR_HARM46_AMP + 4)

#define    BUFF_ADR_HARM47_NO                       (BUFF_ADR_HARM46_PHASE + 4)
#define    BUFF_ADR_HARM47_AMP                      (BUFF_ADR_HARM47_NO + 4)
#define    BUFF_ADR_HARM47_PHASE                    (BUFF_ADR_HARM47_AMP + 4)

#define    BUFF_ADR_HARM48_NO                       (BUFF_ADR_HARM47_PHASE + 4)
#define    BUFF_ADR_HARM48_AMP                      (BUFF_ADR_HARM48_NO + 4)
#define    BUFF_ADR_HARM48_PHASE                    (BUFF_ADR_HARM48_AMP + 4)

#define    BUFF_ADR_HARM49_NO                       (BUFF_ADR_HARM48_PHASE + 4)
#define    BUFF_ADR_HARM49_AMP                      (BUFF_ADR_HARM49_NO + 4)
#define    BUFF_ADR_HARM49_PHASE                    (BUFF_ADR_HARM49_AMP + 4)

#define    BUFF_ADR_HARM50_NO                       (BUFF_ADR_HARM49_PHASE + 4)
#define    BUFF_ADR_HARM50_AMP                      (BUFF_ADR_HARM50_NO + 4)
#define    BUFF_ADR_HARM50_PHASE                    (BUFF_ADR_HARM50_AMP + 4)

#define    BUFF_SET_HARMNO                          (BUFF_ADR_HARM50_PHASE + 4)
#define    BUFF_SET_HARMAMP                         (BUFF_SET_HARMNO + 4)
#define    BUFF_SET_HARMPHASE                       (BUFF_SET_HARMAMP + 4)
#define    BUFF_SET_HARMSRNUM                       (BUFF_SET_HARMPHASE + 4)

#define    BUFF_ADR_MEAS_VOLT                       (BUFF_SET_HARMSRNUM + 4)
#define    BUFF_ADR_MEAS_VDC                        (BUFF_ADR_MEAS_VOLT + 4)
#define    BUFF_ADR_MEAS_VAC                        (BUFF_ADR_MEAS_VDC + 4)
#define    BUFF_ADR_MEAS_IDC                        (BUFF_ADR_MEAS_VAC + 4)
#define    BUFF_ADR_MEAS_I                          (BUFF_ADR_MEAS_IDC + 4)
#define    BUFF_ADR_MEAS_IAC                        (BUFF_ADR_MEAS_I + 4)
#define    BUFF_ADR_MEAS_FREQ                       (BUFF_ADR_MEAS_IAC + 4)

#define    BUFF_ADR_MEAS_VPK_P                      (BUFF_ADR_MEAS_FREQ + 4)
#define    BUFF_ADR_MEAS_IPK_P                      (BUFF_ADR_MEAS_VPK_P + 4)

#define    BUFF_ADR_MEAS_VPK_N                      (BUFF_ADR_MEAS_IPK_P + 4)
#define    BUFF_ADR_MEAS_IPK_N                      (BUFF_ADR_MEAS_VPK_N + 4)

#define    BUFF_ADR_MEAS_V_CF                       (BUFF_ADR_MEAS_IPK_N + 4)
#define    BUFF_ADR_MEAS_I_CF                       (BUFF_ADR_MEAS_V_CF + 4)

#define    BUFF_ADR_MEAS_IS                         (BUFF_ADR_MEAS_I_CF + 4)

#define    BUFF_ADR_MEAS_POWER                      (BUFF_ADR_MEAS_IS + 4)
#define    BUFF_ADR_MEAS_VAR                        (BUFF_ADR_MEAS_POWER + 4)
#define    BUFF_ADR_MEAS_VA                         (BUFF_ADR_MEAS_VAR + 4)
#define    BUFF_ADR_MEAS_PF                         (BUFF_ADR_MEAS_VA + 4)

#define    BUFF_END_ADDR                            (BUFF_ADR_MEAS_PF + 4)

#elif CONVERTER_TYPE == THREE_PHASE
//***************************Source Subsystem************************************//
#define BUFF_ADR_SRC_VA                             1
#define BUFF_ADR_SRC_VB                             BUFF_ADR_SRC_VA + 4
#define BUFF_ADR_SRC_VC                             BUFF_ADR_SRC_VB + 4
#define BUFF_ADR_SRC_VAC                            BUFF_ADR_SRC_VC + 4

#define BUFF_ADR_SRC_VA_SLW                         BUFF_ADR_SRC_VAC + 4
#define BUFF_ADR_SRC_VB_SLW                         BUFF_ADR_SRC_VA_SLW + 4
#define BUFF_ADR_SRC_VC_SLW                         BUFF_ADR_SRC_VB_SLW + 4
#define BUFF_ADR_SRC_VAC_SLW                        BUFF_ADR_SRC_VC_SLW + 4

#define BUFF_ADR_SRC_IA                             BUFF_ADR_SRC_VAC_SLW + 4
#define BUFF_ADR_SRC_IB                             BUFF_ADR_SRC_IA + 4
#define BUFF_ADR_SRC_IC                             BUFF_ADR_SRC_IB + 4
#define BUFF_ADR_SRC_IAC                            BUFF_ADR_SRC_IC + 4

#define BUFF_ADR_SRC_IA_SLW                         BUFF_ADR_SRC_IAC + 4
#define BUFF_ADR_SRC_IB_SLW                         BUFF_ADR_SRC_IA_SLW + 4
#define BUFF_ADR_SRC_IC_SLW                         BUFF_ADR_SRC_IB_SLW + 4
#define BUFF_ADR_SRC_IAC_SLW                        BUFF_ADR_SRC_IC_SLW + 4

#define BUFF_ADR_SRC_FREQ_A                         BUFF_ADR_SRC_IAC_SLW + 4
#define BUFF_ADR_SRC_FREQ_B                         BUFF_ADR_SRC_FREQ_A + 4
#define BUFF_ADR_SRC_FREQ_C                         BUFF_ADR_SRC_FREQ_B + 4
#define BUFF_ADR_SRC_FREQ                           BUFF_ADR_SRC_FREQ_C + 4

#define BUFF_ADR_SRC_FREQ_SLW                       BUFF_ADR_SRC_FREQ + 4
#define BUFF_ADR_SRC_FREQ_A_SLW                     BUFF_ADR_SRC_FREQ_SLW + 4
#define BUFF_ADR_SRC_FREQ_B_SLW                     BUFF_ADR_SRC_FREQ_A_SLW + 4
#define BUFF_ADR_SRC_FREQ_C_SLW                     BUFF_ADR_SRC_FREQ_B_SLW + 4

#define BUFF_ADR_SRC_ANGLE_AB                       BUFF_ADR_SRC_FREQ_C_SLW + 4
#define BUFF_ADR_SRC_ANGLE_AC                       BUFF_ADR_SRC_ANGLE_AB + 4
#define BUFF_ADR_SRC_ANGLE_BC                       BUFF_ADR_SRC_ANGLE_AC + 4

#define BUFF_ADR_SRC_SEQ_ABC                        BUFF_ADR_SRC_ANGLE_BC + 4
#define BUFF_ADR_SRC_SEQ_ACB                        BUFF_ADR_SRC_SEQ_ABC + 4

#define BUFF_ADR_SRC_VA_DC                          BUFF_ADR_SRC_SEQ_ACB + 4
#define BUFF_ADR_SRC_VB_DC                          BUFF_ADR_SRC_VA_DC + 4
#define BUFF_ADR_SRC_VC_DC                          BUFF_ADR_SRC_VB_DC + 4
#define BUFF_ADR_SRC_V_DC                           BUFF_ADR_SRC_VC_DC + 4

#define BUFF_ADR_SRC_VA_DC_SLW                      BUFF_ADR_SRC_V_DC + 4
#define BUFF_ADR_SRC_VB_DC_SLW                      BUFF_ADR_SRC_VA_DC_SLW + 4
#define BUFF_ADR_SRC_VC_DC_SLW                      BUFF_ADR_SRC_VB_DC_SLW + 4
#define BUFF_ADR_SRC_V_DC_SLW                       BUFF_ADR_SRC_VC_DC_SLW + 4

#define BUFF_ADR_SRC_IA_DC                          BUFF_ADR_SRC_V_DC_SLW + 4
#define BUFF_ADR_SRC_IB_DC                          BUFF_ADR_SRC_IA_DC + 4
#define BUFF_ADR_SRC_IC_DC                          BUFF_ADR_SRC_IB_DC + 4
#define BUFF_ADR_SRC_I_DC                           BUFF_ADR_SRC_IC_DC + 4

#define BUFF_ADR_SRC_IA_DC_SLW                      BUFF_ADR_SRC_I_DC + 4
#define BUFF_ADR_SRC_IB_DC_SLW                      BUFF_ADR_SRC_IA_DC_SLW + 4
#define BUFF_ADR_SRC_IC_DC_SLW                      BUFF_ADR_SRC_IB_DC_SLW + 4
#define BUFF_ADR_SRC_I_DC_SLW                       BUFF_ADR_SRC_IC_DC_SLW + 4

#define BUFF_ADR_SRC_ON_DEG_A                       BUFF_ADR_SRC_I_DC_SLW + 4
#define BUFF_ADR_SRC_ON_DEG_B                       BUFF_ADR_SRC_ON_DEG_A + 4
#define BUFF_ADR_SRC_ON_DEG_C                       BUFF_ADR_SRC_ON_DEG_B + 4
#define BUFF_ADR_SRC_ON_DEG_ABC                     BUFF_ADR_SRC_ON_DEG_C + 4

#define BUFF_ADR_SRC_OFF_DEG_A                      BUFF_ADR_SRC_ON_DEG_ABC + 4
#define BUFF_ADR_SRC_OFF_DEG_B                      BUFF_ADR_SRC_OFF_DEG_A + 4
#define BUFF_ADR_SRC_OFF_DEG_C                      BUFF_ADR_SRC_OFF_DEG_B + 4
#define BUFF_ADR_SRC_OFF_DEG_ABC                    BUFF_ADR_SRC_OFF_DEG_C + 4

#define BUFF_ADR_START_PWR_STAGE                    BUFF_ADR_SRC_OFF_DEG_ABC + 4
#define BUFF_ADR_RLY_CTRL                           BUFF_ADR_START_PWR_STAGE + 1

#define BUFF_OUTPUT_MODE                            BUFF_ADR_RLY_CTRL   + 4

//***************************Limit Subsystem************************************//
#define BUFF_ADR_LIM_VA_MAX                         BUFF_OUTPUT_MODE + 1
#define BUFF_ADR_LIM_VA_MIN                         BUFF_ADR_LIM_VA_MAX + 4
#define BUFF_ADR_LIM_VB_MAX                         BUFF_ADR_LIM_VA_MIN + 4
#define BUFF_ADR_LIM_VB_MIN                         BUFF_ADR_LIM_VB_MAX + 4
#define BUFF_ADR_LIM_VC_MAX                         BUFF_ADR_LIM_VB_MIN + 4
#define BUFF_ADR_LIM_VC_MIN                         BUFF_ADR_LIM_VC_MAX + 4
#define BUFF_ADR_LIM_V_MAX                          BUFF_ADR_LIM_VC_MIN + 4
#define BUFF_ADR_LIM_V_MIN                          BUFF_ADR_LIM_V_MAX + 4

#define BUFF_ADR_LIM_IA_MAX                         BUFF_ADR_LIM_V_MIN + 4
#define BUFF_ADR_LIM_IA_MIN                         BUFF_ADR_LIM_IA_MAX + 4
#define BUFF_ADR_LIM_IB_MAX                         BUFF_ADR_LIM_IA_MIN + 4
#define BUFF_ADR_LIM_IB_MIN                         BUFF_ADR_LIM_IB_MAX + 4
#define BUFF_ADR_LIM_IC_MAX                         BUFF_ADR_LIM_IB_MIN + 4
#define BUFF_ADR_LIM_IC_MIN                         BUFF_ADR_LIM_IC_MAX + 4
#define BUFF_ADR_LIM_I_MAX                          BUFF_ADR_LIM_IC_MIN + 4
#define BUFF_ADR_LIM_I_MIN                          BUFF_ADR_LIM_I_MAX + 4

#define BUFF_ADR_LIM_IA_MAX_DLY                     BUFF_ADR_LIM_I_MIN + 4
#define BUFF_ADR_LIM_IA_MIN_DLY                     BUFF_ADR_LIM_IA_MAX_DLY + 4
#define BUFF_ADR_LIM_IB_MAX_DLY                     BUFF_ADR_LIM_IA_MIN_DLY + 4
#define BUFF_ADR_LIM_IB_MIN_DLY                     BUFF_ADR_LIM_IB_MAX_DLY + 4
#define BUFF_ADR_LIM_IC_MAX_DLY                     BUFF_ADR_LIM_IB_MIN_DLY + 4
#define BUFF_ADR_LIM_IC_MIN_DLY                     BUFF_ADR_LIM_IC_MAX_DLY + 4
#define BUFF_ADR_LIM_I_MAX_DLY                      BUFF_ADR_LIM_IC_MIN_DLY + 4
#define BUFF_ADR_LIM_I_MIN_DLY                      BUFF_ADR_LIM_I_MAX_DLY + 4

#define BUFF_ADR_LIM_VA_DC_MAX                      BUFF_ADR_LIM_I_MIN_DLY + 4
#define BUFF_ADR_LIM_VA_DC_MIN                      BUFF_ADR_LIM_VA_DC_MAX + 4
#define BUFF_ADR_LIM_VB_DC_MAX                      BUFF_ADR_LIM_VA_DC_MIN + 4
#define BUFF_ADR_LIM_VB_DC_MIN                      BUFF_ADR_LIM_VB_DC_MAX + 4
#define BUFF_ADR_LIM_VC_DC_MAX                      BUFF_ADR_LIM_VB_DC_MIN + 4
#define BUFF_ADR_LIM_VC_DC_MIN                      BUFF_ADR_LIM_VC_DC_MAX + 4
#define BUFF_ADR_LIM_V_DC_MAX                       BUFF_ADR_LIM_VC_DC_MIN + 4
#define BUFF_ADR_LIM_V_DC_MIN                       BUFF_ADR_LIM_V_DC_MAX + 4

#define BUFF_ADR_LIM_IA_DC_MAX                      BUFF_ADR_LIM_V_DC_MIN + 4
#define BUFF_ADR_LIM_IA_DC_MIN                      BUFF_ADR_LIM_IA_DC_MAX + 4
#define BUFF_ADR_LIM_IB_DC_MAX                      BUFF_ADR_LIM_IA_DC_MIN + 4
#define BUFF_ADR_LIM_IB_DC_MIN                      BUFF_ADR_LIM_IB_DC_MAX + 4
#define BUFF_ADR_LIM_IC_DC_MAX                      BUFF_ADR_LIM_IB_DC_MIN + 4
#define BUFF_ADR_LIM_IC_DC_MIN                      BUFF_ADR_LIM_IC_DC_MAX + 4
#define BUFF_ADR_LIM_I_DC_MAX                       BUFF_ADR_LIM_IC_DC_MIN + 4
#define BUFF_ADR_LIM_I_DC_MIN                       BUFF_ADR_LIM_I_DC_MAX + 4

#define BUFF_ADR_LIM_FREQ_A_MAX                     BUFF_ADR_LIM_I_DC_MIN + 4
#define BUFF_ADR_LIM_FREQ_A_MIN                     BUFF_ADR_LIM_FREQ_A_MAX + 4
#define BUFF_ADR_LIM_FREQ_B_MAX                     BUFF_ADR_LIM_FREQ_A_MIN + 4
#define BUFF_ADR_LIM_FREQ_B_MIN                     BUFF_ADR_LIM_FREQ_B_MAX + 4
#define BUFF_ADR_LIM_FREQ_C_MAX                     BUFF_ADR_LIM_FREQ_B_MIN + 4
#define BUFF_ADR_LIM_FREQ_C_MIN                     BUFF_ADR_LIM_FREQ_C_MAX + 4
#define BUFF_ADR_LIM_FREQ_MAX                       BUFF_ADR_LIM_FREQ_C_MIN + 4
#define BUFF_ADR_LIM_FREQ_MIN                       BUFF_ADR_LIM_FREQ_MAX + 4

#define BUFF_ADR_LIM_PWR_A_MAX                      BUFF_ADR_LIM_FREQ_MIN + 4
#define BUFF_ADR_LIM_PWR_A_MIN                      BUFF_ADR_LIM_PWR_A_MAX + 4
#define BUFF_ADR_LIM_PWR_B_MAX                      BUFF_ADR_LIM_PWR_A_MIN + 4
#define BUFF_ADR_LIM_PWR_B_MIN                      BUFF_ADR_LIM_PWR_B_MAX + 4
#define BUFF_ADR_LIM_PWR_C_MAX                      BUFF_ADR_LIM_PWR_B_MIN + 4
#define BUFF_ADR_LIM_PWR_C_MIN                      BUFF_ADR_LIM_PWR_C_MAX + 4
#define BUFF_ADR_LIM_PWR_TOT_MAX                    BUFF_ADR_LIM_PWR_C_MIN + 4
#define BUFF_ADR_LIM_PWR_TOT_MIN                    BUFF_ADR_LIM_PWR_TOT_MAX + 4

//***************************Measure Subsystem************************************//
#define BUFF_ADR_MEAS_VA_RMS                        BUFF_ADR_LIM_PWR_TOT_MIN + 4
#define BUFF_ADR_MEAS_VB_RMS                        BUFF_ADR_MEAS_VA_RMS + 4
#define BUFF_ADR_MEAS_VC_RMS                        BUFF_ADR_MEAS_VB_RMS + 4
#define BUFF_ADR_MEAS_VAB_RMS                       BUFF_ADR_MEAS_VC_RMS + 4
#define BUFF_ADR_MEAS_VBC_RMS                       BUFF_ADR_MEAS_VAB_RMS + 4
#define BUFF_ADR_MEAS_VCA_RMS                       BUFF_ADR_MEAS_VBC_RMS + 4

#define BUFF_ADR_MEAS_IA_RMS                        BUFF_ADR_MEAS_VCA_RMS + 4
#define BUFF_ADR_MEAS_IB_RMS                        BUFF_ADR_MEAS_IA_RMS + 4
#define BUFF_ADR_MEAS_IC_RMS                        BUFF_ADR_MEAS_IB_RMS + 4
#define BUFF_ADR_MEAS_IAB_RMS                       BUFF_ADR_MEAS_IC_RMS + 4
#define BUFF_ADR_MEAS_IBC_RMS                       BUFF_ADR_MEAS_IAB_RMS + 4
#define BUFF_ADR_MEAS_ICA_RMS                       BUFF_ADR_MEAS_IBC_RMS + 4

#define BUFF_ADR_MEAS_VA_PEAK                       BUFF_ADR_MEAS_ICA_RMS + 4
#define BUFF_ADR_MEAS_VB_PEAK                       BUFF_ADR_MEAS_VA_PEAK + 4
#define BUFF_ADR_MEAS_VC_PEAK                       BUFF_ADR_MEAS_VB_PEAK + 4

#define BUFF_ADR_MEAS_IA_PEAK                       BUFF_ADR_MEAS_VC_PEAK + 4
#define BUFF_ADR_MEAS_IB_PEAK                       BUFF_ADR_MEAS_IA_PEAK + 4
#define BUFF_ADR_MEAS_IC_PEAK                       BUFF_ADR_MEAS_IB_PEAK + 4

#define BUFF_ADR_MEAS_VA_DC                         BUFF_ADR_MEAS_IC_PEAK + 4
#define BUFF_ADR_MEAS_VB_DC                         BUFF_ADR_MEAS_VA_DC + 4
#define BUFF_ADR_MEAS_VC_DC                         BUFF_ADR_MEAS_VB_DC + 4

#define BUFF_ADR_MEAS_IA_DC                         BUFF_ADR_MEAS_VC_DC + 4
#define BUFF_ADR_MEAS_IB_DC                         BUFF_ADR_MEAS_IA_DC + 4
#define BUFF_ADR_MEAS_IC_DC                         BUFF_ADR_MEAS_IB_DC + 4

#define BUFF_ADR_MEAS_FREQ_A                        BUFF_ADR_MEAS_IC_DC + 4
#define BUFF_ADR_MEAS_FREQ_B                        BUFF_ADR_MEAS_FREQ_A + 4
#define BUFF_ADR_MEAS_FREQ_C                        BUFF_ADR_MEAS_FREQ_B + 4

#define BUFF_ADR_MEAS_PWR_A_W                       BUFF_ADR_MEAS_FREQ_C + 4
#define BUFF_ADR_MEAS_PWR_B_W                       BUFF_ADR_MEAS_PWR_A_W + 4
#define BUFF_ADR_MEAS_PWR_C_W                       BUFF_ADR_MEAS_PWR_B_W + 4
#define BUFF_ADR_MEAS_PWR_TOT_W                     BUFF_ADR_MEAS_PWR_C_W + 4

#define BUFF_ADR_MEAS_PWR_A_VAR                     BUFF_ADR_MEAS_PWR_TOT_W + 4
#define BUFF_ADR_MEAS_PWR_B_VAR                     BUFF_ADR_MEAS_PWR_A_VAR + 4
#define BUFF_ADR_MEAS_PWR_C_VAR                     BUFF_ADR_MEAS_PWR_B_VAR + 4
#define BUFF_ADR_MEAS_PWR_TOT_VAR                   BUFF_ADR_MEAS_PWR_C_VAR + 4

#define BUFF_ADR_MEAS_PWR_A_VA                      BUFF_ADR_MEAS_PWR_TOT_VAR + 4
#define BUFF_ADR_MEAS_PWR_B_VA                      BUFF_ADR_MEAS_PWR_A_VA + 4
#define BUFF_ADR_MEAS_PWR_C_VA                      BUFF_ADR_MEAS_PWR_B_VA + 4
#define BUFF_ADR_MEAS_PWR_TOT_VA                    BUFF_ADR_MEAS_PWR_C_VA + 4

#define BUFF_ADR_MEAS_PF_A                          BUFF_ADR_MEAS_PWR_TOT_VA + 4
#define BUFF_ADR_MEAS_PF_B                          BUFF_ADR_MEAS_PF_A + 4
#define BUFF_ADR_MEAS_PF_C                          BUFF_ADR_MEAS_PF_B + 4

//***************************Measure Subsystem INPUT ****************************//
#define BUFF_ADR_INPUT_VA_RMS                       BUFF_ADR_MEAS_PF_C + 4
#define BUFF_ADR_INPUT_VB_RMS                       BUFF_ADR_INPUT_VA_RMS + 4
#define BUFF_ADR_INPUT_VC_RMS                       BUFF_ADR_INPUT_VB_RMS + 4
#define BUFF_ADR_INPUT_VAB_RMS                      BUFF_ADR_INPUT_VC_RMS + 4
#define BUFF_ADR_INPUT_VBC_RMS                      BUFF_ADR_INPUT_VAB_RMS + 4
#define BUFF_ADR_INPUT_VCA_RMS                      BUFF_ADR_INPUT_VBC_RMS + 4

#define BUFF_ADR_INPUT_IA_RMS                       BUFF_ADR_INPUT_VCA_RMS + 4
#define BUFF_ADR_INPUT_IB_RMS                       BUFF_ADR_INPUT_IA_RMS + 4
#define BUFF_ADR_INPUT_IC_RMS                       BUFF_ADR_INPUT_IB_RMS + 4
#define BUFF_ADR_INPUT_IAB_RMS                      BUFF_ADR_INPUT_IC_RMS + 4
#define BUFF_ADR_INPUT_IBC_RMS                      BUFF_ADR_INPUT_IAB_RMS + 4
#define BUFF_ADR_INPUT_ICA_RMS                      BUFF_ADR_INPUT_IBC_RMS + 4

#define BUFF_ADR_INPUT_FREQ_A                       BUFF_ADR_INPUT_ICA_RMS + 4
#define BUFF_ADR_INPUT_FREQ_B                       BUFF_ADR_INPUT_FREQ_A + 4
#define BUFF_ADR_INPUT_FREQ_C                       BUFF_ADR_INPUT_FREQ_B + 4

#define BUFF_ADR_INPUT_PWR_A_W                      BUFF_ADR_INPUT_FREQ_C + 4
#define BUFF_ADR_INPUT_PWR_B_W                      BUFF_ADR_INPUT_PWR_A_W + 4
#define BUFF_ADR_INPUT_PWR_C_W                      BUFF_ADR_INPUT_PWR_B_W + 4
#define BUFF_ADR_INPUT_PWR_TOT_W                    BUFF_ADR_INPUT_PWR_C_W + 4

#define BUFF_ADR_INPUT_PWR_A_VAR                    BUFF_ADR_INPUT_PWR_TOT_W + 4
#define BUFF_ADR_INPUT_PWR_B_VAR                    BUFF_ADR_INPUT_PWR_A_VAR + 4
#define BUFF_ADR_INPUT_PWR_C_VAR                    BUFF_ADR_INPUT_PWR_B_VAR + 4
#define BUFF_ADR_INPUT_PWR_TOT_VAR                  BUFF_ADR_INPUT_PWR_C_VAR + 4

#define BUFF_ADR_INPUT_PWR_A_VA                     BUFF_ADR_INPUT_PWR_TOT_VAR + 4
#define BUFF_ADR_INPUT_PWR_B_VA                     BUFF_ADR_INPUT_PWR_A_VA + 4
#define BUFF_ADR_INPUT_PWR_C_VA                     BUFF_ADR_INPUT_PWR_B_VA + 4
#define BUFF_ADR_INPUT_PWR_TOT_VA                   BUFF_ADR_INPUT_PWR_C_VA + 4

#define BUFF_ADR_INPUT_PF_A                         BUFF_ADR_INPUT_PWR_TOT_VA + 4
#define BUFF_ADR_INPUT_PF_B                         BUFF_ADR_INPUT_PF_A + 4
#define BUFF_ADR_INPUT_PF_C                         BUFF_ADR_INPUT_PF_B + 4

#define BUFF_ADR_INPUT_VDC_FB                       BUFF_ADR_INPUT_PF_C + 4
#define BUFF_ADR_EFFECIENCY                         BUFF_ADR_INPUT_VDC_FB + 4

#define BUFF_END_ADDR                               BUFF_ADR_EFFECIENCY + 4

#endif


extern void InitDataMappping(void);
extern void MarkDirty(uint16_t address);
extern void ProcessData(void);
float32_t hex2float(void);
int32_t hex2int(void);
extern void ReadMeasureDataFromSharedMemory(void);
extern void MemDataUpdate(void);
extern void HarmonicTableUpdate(uint16_t startIdx, uint16_t length);

extern const DataMapEntry Measure_Input_dataMap[];
extern const size_t NUM_ENTRIES_MEAS_IN;

#endif /* COMMON_MEMMAP_H_ */
