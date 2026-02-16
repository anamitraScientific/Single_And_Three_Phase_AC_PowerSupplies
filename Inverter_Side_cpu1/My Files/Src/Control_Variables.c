/*
 * Control_Variables.c
 *
 *  Created on: Jul 21, 2025
 *      Author: Anamitra
 */

#include "Control_Variables.h"

#if CONVERTER_TYPE == SINGLE_PHASE

float32_t Vac_fundamental;
float32_t V_DC;
float32_t AC_Freq_Ref;
float32_t slope_VacRef;
float32_t V_DC_slope;
float32_t slope_FreqRef;
volatile int32_t StartPowerStage;
volatile int32_t StartPowerStage_prev;
float32_t ON_degree;
float32_t OFF_degree;
volatile int32_t CouplingMode;

float32_t Limit_VAC             = 0.0f;
float32_t Limit_VDC_max         = 0.0f;
float32_t Limit_VDC_min         = 0.0f;
float32_t Limit_AC_Freq_Ref     = 0.0f;
float32_t Limit_OPP             = 0.0f;
float32_t Limit_OCP             = 0.0f;
float32_t Limit_OCP_Delay       = 0.0f;

/* MEASUREMENT VARIABLES */
float32_t Meas_Vrms             = 0.0f;         // RMS Voltage
float32_t Meas_Vdc              = 0.0f;         // DC Voltage component
float32_t Meas_Vac              = 0.0f;         // AC Voltage component
float32_t Meas_Idc              = 0.0f;         // DC Current component
float32_t Meas_Irms             = 0.0f;         // RMS Current
float32_t Meas_Iac              = 0.0f;         // AC Current component
float32_t Meas_Freq             = 0.0f;         // Measured Frequency

float32_t Meas_Vpk_P            = 0.0f;         // Peak Voltage
float32_t Meas_Ipk_P            = 0.0f;         // Peak Current

float32_t Meas_Vpk_N            = 0.0f;         // Peak Voltage
float32_t Meas_Ipk_N            = 0.0f;         // Peak Current

float32_t Meas_V_CF             = 0.0f;         // Crest Factor
float32_t Meas_I_CF             = 0.0f;         // Crest Factor

float32_t Meas_Is               = 0.0f;         // Surge Current / Inrush Current

float32_t Meas_Preal            = 0.0f;         // Real Power (Watts)
float32_t Meas_Preactive        = 0.0f;         // Reactive Power (VAR)
float32_t Meas_Papparent        = 0.0f;         // Apparent Power (VA)
float32_t Meas_PF               = 0.0f;         // Power Factor


//float32_t Vdc_fb                = 0.0f;

#elif CONVERTER_TYPE == THREE_PHASE

//***********Source Subsystem***********//
float32_t VA_fundamental;//done
float32_t VB_fundamental;//done
float32_t VC_fundamental;//done
float32_t Vac_fundamental;//done

float32_t slope_VaRef;//done
float32_t slope_VbRef;//done
float32_t slope_VcRef;//done
float32_t slope_VacRef;//done

float32_t IA_fundamental;//done
float32_t IB_fundamental;//done
float32_t IC_fundamental;//done
float32_t Iac_fundamental;//done

float32_t slope_IaRef;//done
float32_t slope_IbRef;//done
float32_t slope_IcRef;//done
float32_t slope_IacRef;//done

float32_t AC_Freq_Ref_A;//done
float32_t AC_Freq_Ref_B;//done
float32_t AC_Freq_Ref_C;//done
float32_t AC_Freq_Ref;//done

float32_t slope_FreqRef;//done
float32_t slope_FreqRefA;//done
float32_t slope_FreqRefB;//done
float32_t slope_FreqRefC;//done

float32_t Phase_angle_AB;//to be checked later with original variables
float32_t Phase_angle_AC;//to be checked later with original variables
float32_t Phase_angle_BC;//to be checked later with original variables

float32_t Phase_seq_ABC;//new variables
float32_t Phase_seq_ACB;//new variables

float32_t VA_DC;//new variables
float32_t VB_DC;//new variables
float32_t VC_DC;//new variables
float32_t V_DC;//new variables

float32_t VA_DC_slope;//new variables
float32_t VB_DC_slope;//new variables
float32_t VC_DC_slope;//new variables
float32_t V_DC_slope;//new variables

float32_t IA_DC;//new variables
float32_t IB_DC;//new variables
float32_t IC_DC;//new variables
float32_t I_DC;//new variables

float32_t IA_DC_slope;//new variables
float32_t IB_DC_slope;//new variables
float32_t IC_DC_slope;//new variables
float32_t I_DC_slope;//new variables

float32_t ON_degree_A;//new variables
float32_t ON_degree_B;//new variables
float32_t ON_degree_C;//new variables
float32_t ON_degree_ABC;//new variables

float32_t OFF_degree_A;//new variables
float32_t OFF_degree_B;//new variables
float32_t OFF_degree_C;//new variables
float32_t OFF_degree_ABC;//new variables

volatile int32_t StartPowerStage;//done

volatile int32_t StartPowerStage_temp;
volatile int32_t StartPowerStage_prev;

volatile  int32_t Relay_Ctrl;
volatile int32_t OutputMode;

//***********Limit Subsystem**********//


float32_t VA_fundamental_max;//new variables
float32_t VA_fundamental_min;//new variables
float32_t VB_fundamental_max;//new variables
float32_t VB_fundamental_min;//new variables
float32_t VC_fundamental_max;//new variables
float32_t VC_fundamental_min;//new variables
float32_t V_fundamental_max;//new variables
float32_t V_fundamental_min;//new variables

float32_t IA_fundamental_max;//new variables
float32_t IA_fundamental_min;//new variables
float32_t IB_fundamental_max;//new variables
float32_t IB_fundamental_min;//new variables
float32_t IC_fundamental_max;//new variables
float32_t IC_fundamental_min;//new variables
float32_t I_fundamental_max;//new variables
float32_t I_fundamental_min;//new variables

float32_t IA_fundamental_max_delay;//new variables
float32_t IA_fundamental_min_delay;//new variables
float32_t IB_fundamental_max_delay;//new variables
float32_t IB_fundamental_min_delay;//new variables
float32_t IC_fundamental_max_delay;//new variables
float32_t IC_fundamental_min_delay;//new variables
float32_t I_fundamental_max_delay;//new variables
float32_t I_fundamental_min_delay;//new variables

float32_t VA_DC_max;//new variables
float32_t VA_DC_min;//new variables
float32_t VB_DC_max;//new variables
float32_t VB_DC_min;//new variables
float32_t VC_DC_max;//new variables
float32_t VC_DC_min;//new variables
float32_t V_DC_max;//new variables
float32_t V_DC_min;//new variables

float32_t IA_DC_max;//new variables
float32_t IA_DC_min;//new variables
float32_t IB_DC_max;//new variables
float32_t IB_DC_min;//new variables
float32_t IC_DC_max;//new variables
float32_t IC_DC_min;//new variables
float32_t I_DC_max;//new variables
float32_t I_DC_min;//new variables

float32_t AC_Freq_Ref_A_max;//new variables
float32_t AC_Freq_Ref_A_min;//new variables
float32_t AC_Freq_Ref_B_max;//new variables
float32_t AC_Freq_Ref_B_min;//new variables
float32_t AC_Freq_Ref_C_max;//new variables
float32_t AC_Freq_Ref_C_min;//new variables
float32_t AC_Freq_Ref_max;//new variables
float32_t AC_Freq_Ref_min;//new variables

float32_t powerRms_A_Watts_max;//new variables
float32_t powerRms_A_Watts_min;//new variables
float32_t powerRms_B_Watts_max;//new variables
float32_t powerRms_B_Watts_min;//new variables
float32_t powerRms_C_Watts_max;//new variables
float32_t powerRms_C_Watts_min;//new variables
float32_t powerRmsTotal_Watts_max;//new variables
float32_t powerRmsTotal_Watts_min;//new variables

//************Measure Subsytem************//

float32_t VA_RMS_meas;//old variables with new name
float32_t VB_RMS_meas;//old variables with new name
float32_t VC_RMS_meas;//old variables with new name
float32_t VAB_RMS_meas;//new variables
float32_t VBC_RMS_meas;//new variables
float32_t VCA_RMS_meas;//new variables
float32_t IA_RMS_meas;//old variables with new name
float32_t IB_RMS_meas;//old variables with new name
float32_t IC_RMS_meas;//old variables with new name
float32_t IAB_RMS_meas;//new variables
float32_t IBC_RMS_meas;//new variables
float32_t ICA_RMS_meas;//new variables

float32_t VA_PEAK_meas;//new variables
float32_t VB_PEAK_meas;//new variables
float32_t VC_PEAK_meas;//new variables

float32_t IA_PEAK_meas;//old variables with new name
float32_t IB_PEAK_meas;//old variables with new name
float32_t IC_PEAK_meas;//old variables with new name


float32_t VA_DC_meas;//new variables
float32_t VB_DC_meas;//new variables
float32_t VC_DC_meas;//new variables
float32_t IA_DC_meas;//new variables
float32_t IB_DC_meas;//new variables
float32_t IC_DC_meas;//new variables

float32_t Freq_Meas_A;//new variable with 3 individual frequency
float32_t Freq_Meas_B;//new variable with 3 individual frequency
float32_t Freq_Meas_C;//new variable with 3 individual frequency

float32_t powerRms_A_Watts;//done
float32_t powerRms_B_Watts;//done
float32_t powerRms_C_Watts;//done
float32_t powerRmsTotal_Watts;//done

float32_t powerRms_A_VARS;//new variable
float32_t powerRms_B_VARS;//new variable
float32_t powerRms_C_VARS;//new variable
float32_t powerRmsTotal_VARS;//new variable

float32_t powerRms_A_VA;//new variable
float32_t powerRms_B_VA;//new variable
float32_t powerRms_C_VA;//new variable
float32_t powerRmsTotal_VA;//new variable

float32_t powerFactor_A;//done
float32_t powerFactor_B;//done
float32_t powerFactor_C;//done

//************Measure Subsystem INPUT ************//

float32_t VA_RMS_INPUT_meas;//new variables
float32_t VB_RMS_INPUT_meas;//new variables
float32_t VC_RMS_INPUT_meas;//new variables
float32_t VAB_RMS_INPUT_meas;//new variables
float32_t VBC_RMS_INPUT_meas;//new variables
float32_t VCA_RMS_INPUT_meas;//new variables
float32_t IA_RMS_INPUT_meas;//new variables
float32_t IB_RMS_INPUT_meas;//new variables
float32_t IC_RMS_INPUT_meas;//new variables
float32_t IAB_RMS_INPUT_meas;//new variables
float32_t IBC_RMS_INPUT_meas;//new variables
float32_t ICA_RMS_INPUT_meas;//new variables

float32_t Freq_INPUT_Meas_A;//new variables
float32_t Freq_INPUT_Meas_B;//new variables
float32_t Freq_INPUT_Meas_C;//new variables

float32_t powerRms_A_Input_Watts;//new variables
float32_t powerRms_B_Input_Watts;//new variables
float32_t powerRms_C_Input_Watts;//new variables
float32_t powerRmsTotal_Input_Watts;//new variables

float32_t powerRms_A_Input_VARS;//new variables
float32_t powerRms_B_Input_VARS;//new variables
float32_t powerRms_C_Input_VARS;//new variables
float32_t powerRmsTotal_Input_VARS;//new variables

float32_t powerRms_A_Input_VA;//new variables
float32_t powerRms_B_Input_VA;//new variables
float32_t powerRms_C_Input_VA;//new variables
float32_t powerRmsTotal_Input_VA;//new variables

float32_t powerFactor_INPUT_A;//new variables
float32_t powerFactor_INPUT_B;//new variables
float32_t powerFactor_INPUT_C;//new variables

float32_t Vdc_fb;//done
float32_t efficiency;

#endif

bool seq_start;
float32_t seq_table[100][3];



















