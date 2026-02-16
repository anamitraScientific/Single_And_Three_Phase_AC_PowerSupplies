/*
 * Common_Memmap.c
 *
 *  Created on: Jul 22, 2025
 *      Author: Anamitra Sarkar
 */


#include "Common_Memmap.h"


uint8_t data[4];

uint16_t addr_prev;
float32_t val_prev_float;
int32_t val_prev_int;

FloatToUint16 floatConv;
IntToUint16 intConv;


static const DataMapEntry Source_Limit_dataMap[] = {

#if CONVERTER_TYPE == SINGLE_PHASE
                                                     //Source Subsystem
                                                     {BUFF_ADR_SET_VAC, &Vac_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SET_VDC, &V_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SET_FREQ, &AC_Freq_Ref, TYPE_FLOAT32},
                                                     {BUFF_ADR_VAC_SLEW, &slope_VacRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_VDC_SLEW, &V_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_FREQ_SLEW, &slope_FreqRef, TYPE_FLOAT32},

                                                     {BUFF_ADR_OUT_STATE, &StartPowerStage, TYPE_INT32},

                                                     {BUFF_ADR_ON_DEGREE, &ON_degree, TYPE_FLOAT32},
                                                     {BUFF_ADR_OFF_DEGREE, &OFF_degree, TYPE_FLOAT32},

                                                     {BUFF_ADR_COUPLE, &CouplingMode, TYPE_INT32},


                                                     //Limit SubSystem

                                                     {BUFF_ADR_LIM_VAC, &Limit_VAC, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VDC_P, &Limit_VDC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VDC_M, &Limit_VDC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ, &Limit_AC_Freq_Ref, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_OPP, &Limit_OPP, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_OCP, &Limit_OCP, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_OCP_DLY, &Limit_OCP_Delay, TYPE_FLOAT32},
#elif CONVERTER_TYPE == THREE_PHASE
                                                     //Source Subsystem
                                                     {BUFF_ADR_SRC_VA, &VA_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VB, &VB_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VC, &VC_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VAC, &Vac_fundamental, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_VA_SLW, &slope_VaRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VB_SLW, &slope_VbRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VC_SLW, &slope_VcRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VAC_SLW, &slope_VacRef, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_IA, &IA_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IB, &IB_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IC, &IC_fundamental, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IAC, &Iac_fundamental, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_IA_SLW, &slope_IaRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IB_SLW, &slope_IbRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IC_SLW, &slope_IcRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IAC_SLW, &slope_IacRef, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_FREQ_A, &AC_Freq_Ref_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_FREQ_B, &AC_Freq_Ref_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_FREQ_C, &AC_Freq_Ref_C, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_FREQ, &AC_Freq_Ref, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_FREQ_SLW, &slope_FreqRef, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_FREQ_A_SLW, &slope_FreqRefA, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_FREQ_B_SLW, &slope_FreqRefB, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_FREQ_C_SLW, &slope_FreqRefC, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_ANGLE_AB, &Phase_angle_AB, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_ANGLE_AC, &Phase_angle_AC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_ANGLE_BC, &Phase_angle_BC, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_SEQ_ABC, &Phase_seq_ABC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_SEQ_ACB, &Phase_seq_ACB, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_VA_DC, &VA_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VB_DC, &VB_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VC_DC, &VC_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_V_DC, &V_DC, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_VA_DC_SLW, &VA_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VB_DC_SLW, &VB_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_VC_DC_SLW, &VC_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_V_DC_SLW, &V_DC_slope, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_IA_DC, &IA_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IB_DC, &IB_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IC_DC, &IC_DC, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_I_DC, &I_DC, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_IA_DC_SLW, &IA_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IB_DC_SLW, &IB_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_IC_DC_SLW, &IC_DC_slope, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_I_DC_SLW, &I_DC_slope, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_ON_DEG_A, &ON_degree_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_ON_DEG_B, &ON_degree_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_ON_DEG_C, &ON_degree_C, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_ON_DEG_ABC, &ON_degree_ABC, TYPE_FLOAT32},

                                                     {BUFF_ADR_SRC_OFF_DEG_A, &OFF_degree_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_OFF_DEG_B, &OFF_degree_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_OFF_DEG_C, &OFF_degree_C, TYPE_FLOAT32},
                                                     {BUFF_ADR_SRC_OFF_DEG_ABC, &OFF_degree_ABC, TYPE_FLOAT32},

                                                     {BUFF_ADR_START_PWR_STAGE, &StartPowerStage, TYPE_INT32},
                                                     {BUFF_ADR_RLY_CTRL, &Relay_Ctrl, TYPE_INT32},
                                                     {BUFF_OUTPUT_MODE, &OutputMode, TYPE_INT32},

                                                     //Limit SubSystem

                                                     {BUFF_ADR_LIM_VA_MAX, &VA_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VA_MIN, &VA_fundamental_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VB_MAX, &VB_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VB_MIN, &VB_fundamental_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VC_MAX, &VC_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VC_MIN, &VC_fundamental_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_V_MAX, &V_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_V_MIN, &V_fundamental_min, TYPE_FLOAT32},

                                                     {BUFF_ADR_LIM_IA_MAX, &IA_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IA_MIN, &IA_fundamental_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IB_MAX, &IB_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IB_MIN, &IB_fundamental_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IC_MAX, &IC_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IC_MIN, &IC_fundamental_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_I_MAX, &I_fundamental_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_I_MIN, &I_fundamental_min, TYPE_FLOAT32},

                                                     {BUFF_ADR_LIM_IA_MAX_DLY, &IA_fundamental_max_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IA_MIN_DLY, &IA_fundamental_min_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IB_MAX_DLY, &IB_fundamental_max_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IB_MIN_DLY, &IB_fundamental_min_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IC_MAX_DLY, &IC_fundamental_max_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IC_MIN_DLY, &IC_fundamental_min_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_I_MAX_DLY, &I_fundamental_max_delay, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_I_MIN_DLY, &I_fundamental_min_delay, TYPE_FLOAT32},

                                                     {BUFF_ADR_LIM_VA_DC_MAX, &VA_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VA_DC_MIN, &VA_DC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VB_DC_MAX, &VB_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VB_DC_MIN, &VB_DC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VC_DC_MAX, &VC_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_VC_DC_MIN, &VC_DC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_V_DC_MAX, &V_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_V_DC_MIN, &V_DC_min, TYPE_FLOAT32},

                                                     {BUFF_ADR_LIM_IA_DC_MAX, &IA_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IA_DC_MIN, &IA_DC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IB_DC_MAX, &IB_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IB_DC_MIN, &IB_DC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IC_DC_MAX, &IC_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_IC_DC_MIN, &IC_DC_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_I_DC_MAX, &I_DC_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_I_DC_MIN, &I_DC_min, TYPE_FLOAT32},

                                                     {BUFF_ADR_LIM_FREQ_A_MAX, &AC_Freq_Ref_A_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_A_MIN, &AC_Freq_Ref_A_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_B_MAX, &AC_Freq_Ref_B_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_B_MIN, &AC_Freq_Ref_B_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_C_MAX, &AC_Freq_Ref_C_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_C_MIN, &AC_Freq_Ref_C_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_MAX, &AC_Freq_Ref_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_FREQ_MIN, &AC_Freq_Ref_min, TYPE_FLOAT32},

                                                     {BUFF_ADR_LIM_PWR_A_MAX, &powerRms_A_Watts_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_A_MIN, &powerRms_A_Watts_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_B_MAX, &powerRms_B_Watts_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_B_MIN, &powerRms_B_Watts_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_C_MAX, &powerRms_C_Watts_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_C_MIN, &powerRms_C_Watts_min, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_TOT_MAX, &powerRmsTotal_Watts_max, TYPE_FLOAT32},
                                                     {BUFF_ADR_LIM_PWR_TOT_MIN, &powerRmsTotal_Watts_min, TYPE_FLOAT32},
#endif
};

static const DataMapEntry Measure_Input_dataMap[] = {

#if CONVERTER_TYPE == SINGLE_PHASE

                                                     //Measure Subsystem
                                                     {BUFF_ADR_MEAS_VOLT, &Meas_Vrms, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VDC, &Meas_Vdc, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VAC, &Meas_Vac, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IDC, &Meas_Idc, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_I, &Meas_Irms, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IAC, &Meas_Iac, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_FREQ, &Meas_Freq, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_VPK_P, &Meas_Vpk_P, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IPK_P, &Meas_Ipk_P, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_VPK_N, &Meas_Vpk_N, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IPK_N, &Meas_Ipk_N, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_V_CF, &Meas_V_CF, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_I_CF, &Meas_I_CF, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_IS, &Meas_Is, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_POWER, &Meas_Preal, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VAR, &Meas_Preactive, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VA, &Meas_Papparent, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PF, &Meas_PF, TYPE_FLOAT32},
#elif CONVERTER_TYPE == THREE_PHASE
                                                     //Measure Subsystem
                                                     {BUFF_ADR_MEAS_VA_RMS, &VA_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VB_RMS, &VB_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VC_RMS, &VC_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VAB_RMS, &VAB_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VBC_RMS, &VBC_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VCA_RMS, &VCA_RMS_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_IA_RMS, &IA_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IB_RMS, &IB_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IC_RMS, &IC_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IAB_RMS, &IAB_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IBC_RMS, &IBC_RMS_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_ICA_RMS, &ICA_RMS_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_VA_PEAK, &VA_PEAK_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VB_PEAK, &VB_PEAK_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VC_PEAK, &VC_PEAK_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_IA_PEAK, &IA_PEAK_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IB_PEAK, &IB_PEAK_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IC_PEAK, &IC_PEAK_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_VA_DC, &VA_DC_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VB_DC, &VB_DC_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_VC_DC, &VC_DC_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_IA_DC, &IA_DC_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IB_DC, &IB_DC_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_IC_DC, &IC_DC_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_FREQ_A, &Freq_Meas_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_FREQ_B, &Freq_Meas_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_FREQ_C, &Freq_Meas_C, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_PWR_A_W, &powerRms_A_Watts, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_B_W, &powerRms_B_Watts, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_C_W, &powerRms_C_Watts, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_TOT_W, &powerRmsTotal_Watts, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_PWR_A_VAR, &powerRms_A_VARS, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_B_VAR, &powerRms_B_VARS, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_C_VAR, &powerRms_C_VARS, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_TOT_VAR, &powerRmsTotal_VARS, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_PWR_A_VA, &powerRms_A_VA, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_B_VA, &powerRms_B_VA, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_C_VA, &powerRms_C_VA, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PWR_TOT_VA, &powerRmsTotal_VA, TYPE_FLOAT32},

                                                     {BUFF_ADR_MEAS_PF_A, &powerFactor_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PF_B, &powerFactor_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_MEAS_PF_C, &powerFactor_C, TYPE_FLOAT32},

                                                     //Measure Input Subsystem

                                                     {BUFF_ADR_INPUT_VA_RMS, &VA_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_VB_RMS, &VB_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_VC_RMS, &VC_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_VAB_RMS, &VAB_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_VBC_RMS, &VBC_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_VCA_RMS, &VCA_RMS_INPUT_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_IA_RMS, &IA_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_IB_RMS, &IB_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_IC_RMS, &IC_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_IAB_RMS, &IAB_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_IBC_RMS, &IBC_RMS_INPUT_meas, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_ICA_RMS, &ICA_RMS_INPUT_meas, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_FREQ_A, &Freq_INPUT_Meas_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_FREQ_B, &Freq_INPUT_Meas_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_FREQ_C, &Freq_INPUT_Meas_C, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_PWR_A_W, &powerRms_A_Input_Watts, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_B_W, &powerRms_B_Input_Watts, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_C_W, &powerRms_C_Input_Watts, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_TOT_W, &powerRmsTotal_Input_Watts, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_PWR_A_VAR, &powerRms_A_Input_VARS, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_B_VAR, &powerRms_B_Input_VARS, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_C_VAR, &powerRms_C_Input_VARS, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_TOT_VAR, &powerRmsTotal_Input_VARS, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_PWR_A_VA, &powerRms_A_Input_VA, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_B_VA, &powerRms_B_Input_VA, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_C_VA, &powerRms_C_Input_VA, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PWR_TOT_VA, &powerRmsTotal_Input_VA, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_PF_A, &powerFactor_INPUT_A, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PF_B, &powerFactor_INPUT_B, TYPE_FLOAT32},
                                                     {BUFF_ADR_INPUT_PF_C, &powerFactor_INPUT_C, TYPE_FLOAT32},

                                                     {BUFF_ADR_INPUT_VDC_FB, &Vdc_fb, TYPE_FLOAT32},
                                                     {BUFF_ADR_EFFECIENCY, &efficiency, TYPE_FLOAT32},
#endif
};

#define NUM_ENTRIES (sizeof(Source_Limit_dataMap)/sizeof(DataMapEntry))


static uint8_t dirtyFlags[NUM_ENTRIES];

void InitSource_dataMappping(void)
{
    int i = 0;
    for(i = 0; i < NUM_ENTRIES; i++)
    {
        dirtyFlags[i] = 0;
    }
}

void MarkDirty(uint16_t address)
{
    int i = 0;
    for(i = 0; i < NUM_ENTRIES; i++)
    {
        if(Source_Limit_dataMap[i].BUF_ADR == address)
        {
            dirtyFlags[i] = 1;
            break;
        }
    }
}


void ReadingDataFromSharedMemory(void)
{
    uint16_t addr;
    float32_t val_Now_float;
    int32_t val_Now_int;


    int i = 0;
    for(i = 0; i < NUM_ENTRIES;i++)
    {
        if(dirtyFlags[i])
        {
            addr = Source_Limit_dataMap[i].BUF_ADR;

            data[0] = cpu1Read[addr];
            data[1] = cpu1Read[addr + 1];
            data[2] = cpu1Read[addr + 2];
            data[3] = cpu1Read[addr + 3];

            switch(Source_Limit_dataMap[i].type)
            {
            case TYPE_FLOAT32:
                val_Now_float = hex2float();
                if(val_Now_float == val_prev_float && addr == addr_prev)
                {

                }
                else
                {
                    *(volatile float32_t *)(Source_Limit_dataMap[i].variable_ptr) = val_Now_float;
                    val_prev_float = val_Now_float;
                    addr_prev = addr;
                }
                break;
            case TYPE_INT32:
                val_Now_int = hex2int();
                if(val_Now_int == val_prev_int && addr == addr_prev)
                {

                }
                else
                {
                    *(volatile int32_t *)(Source_Limit_dataMap[i].variable_ptr) = val_Now_int;
                    val_prev_int = val_Now_int;
                    addr_prev = addr;
                }
            }
            dirtyFlags[i] = 0;
            break;
        }
    }
}

void MemDataUpdate()
{
    uint16_t addr;
    int i = 0;
    for(i = 0; i < NUM_ENTRIES; i ++)
    {
        addr = Source_Limit_dataMap[i].BUF_ADR;

        data[0] = cpu1Read[addr];
        data[1] = cpu1Read[addr + 1];
        data[2] = cpu1Read[addr + 2];
        data[3] = cpu1Read[addr + 3];

        switch(Source_Limit_dataMap[i].type)
        {
        case TYPE_FLOAT32:
            *(volatile float32_t *)(Source_Limit_dataMap[i].variable_ptr) = hex2float();
            break;
        case TYPE_INT32:
            *(volatile int32_t *)(Source_Limit_dataMap[i].variable_ptr) = hex2int();
        }
    }
}

float32_t hex2float(void)
{
    float32_t datareturn;

    floatConv.int16data[0] = (data[1] * 256) +data[0];
    floatConv.int16data[1] = (data[3] * 256) + data[2];

    datareturn = floatConv.floatVal;

    return datareturn;
}

int32_t hex2int(void)
{
    int32_t datareturn;

    intConv.int16data[0] = (data[1] * 256) +data[0];
    intConv.int16data[0] = (data[1] * 256) +data[0];

    datareturn = intConv.intVal;

    return datareturn;
}


void WriteMeasureDataToSharedMemory(void)
{
    union{
        float32_t f;
        int32_t i;
        uint8_t u[4];
    }DataConv;

    uint16_t i = 0;

    for(i = 0; i < (sizeof(Measure_Input_dataMap)/sizeof(DataMapEntry)); i++)
    {
        const DataMapEntry *entry = &Measure_Input_dataMap[i];

        switch(entry->type)
        {
        case TYPE_FLOAT32:
            DataConv.f = *(volatile float32_t *)(entry->variable_ptr);
            break;
        case TYPE_INT32:
            DataConv.i = *(volatile int32_t *)(entry->variable_ptr);
            break;
        }

//        cpu1Write[entry->BUF_ADR] = DataConv.u[0];
//        cpu1Write[entry->BUF_ADR + 1] = DataConv.u[1];
//        cpu1Write[entry->BUF_ADR + 2] = DataConv.u[2];
//        cpu1Write[entry->BUF_ADR + 3] = DataConv.u[3];

        cpu1Write[entry->BUF_ADR] = (DataConv.u[0] & 0x00FF);
        cpu1Write[entry->BUF_ADR + 1] = ((DataConv.u[0] >> 8) & 0x00FF);
        cpu1Write[entry->BUF_ADR + 2] = (DataConv.u[1] & 0x00FF);
        cpu1Write[entry->BUF_ADR + 3] = ((DataConv.u[1] >> 8) & 0x00FF);
    }
}












