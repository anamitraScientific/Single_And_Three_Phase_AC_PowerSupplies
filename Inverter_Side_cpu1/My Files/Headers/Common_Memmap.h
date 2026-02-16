/*
 * Common_Memmap.h
 *
 *  Created on: Jul 22, 2025
 *      Author: admin
 */

#ifndef COMMON_MEMMAP_H_
#define COMMON_MEMMAP_H_

#include <stdio.h>
#include <stdint.h>
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

extern uint16_t addr_prev;
extern float32_t val_prev_float;
extern int32_t val_prev_int;

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

#define    BUFF_ADR_MEAS_VOLT                       (BUFF_ADR_LIM_OCP_DLY + 4)
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

#endif


extern void InitDataMappping(void);
extern void MarkDirty(uint16_t address);
extern void ReadingDataFromSharedMemory(void);
float32_t hex2float(void);
int32_t hex2int(void);
extern void WriteMeasureDataToSharedMemory(void);
extern void MemDataUpdate();




#endif /* COMMON_MEMMAP_H_ */
