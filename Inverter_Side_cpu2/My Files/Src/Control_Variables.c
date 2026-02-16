/*
 * Control_Variables.c
 *
 *  Created on: Jul 18, 2025
 *      Author: Anamitra Sarkar
*/

#include "Control_Variables.h"

#if CONVERTER_TYPE == SINGLE_PHASE

volatile int32_t DSP_boot       = 0;
float32_t Vac_fundamental       = 0.0f;
float32_t V_DC                  = 0.0f;
float32_t AC_Freq_Ref           = 0.0f;
float32_t slope_VacRef          = 0.0f;
float32_t V_DC_slope            = 0.0f;
float32_t slope_FreqRef         = 0.0f;
volatile int32_t StartPowerStage = 0;
float32_t ON_degree             = 0.0f;
float32_t OFF_degree            = 0.0f;
volatile int32_t CouplingMode   = 0;

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

#elif CONVERTER_TYPE == THREE_PHASE

//***********Source Subsystem***********//
float32_t VA_fundamental;
float32_t VB_fundamental;
float32_t VC_fundamental;
float32_t Vac_fundamental;

float32_t slope_VaRef;
float32_t slope_VbRef;
float32_t slope_VcRef;
float32_t slope_VacRef;

float32_t IA_fundamental;
float32_t IB_fundamental;
float32_t IC_fundamental;
float32_t Iac_fundamental;

float32_t slope_IaRef;
float32_t slope_IbRef;
float32_t slope_IcRef;
float32_t slope_IacRef;


float32_t AC_Freq_Ref_A;
float32_t AC_Freq_Ref_B;
float32_t AC_Freq_Ref_C;
float32_t AC_Freq_Ref;

float32_t slope_FreqRef;
float32_t slope_FreqRefA;
float32_t slope_FreqRefB;
float32_t slope_FreqRefC;

float32_t Phase_angle_AB;
float32_t Phase_angle_AC;
float32_t Phase_angle_BC;

float32_t Phase_seq_ABC;
float32_t Phase_seq_ACB;

float32_t VA_DC;
float32_t VB_DC;
float32_t VC_DC;
float32_t V_DC;

float32_t VA_DC_slope;
float32_t VB_DC_slope;
float32_t VC_DC_slope;
float32_t V_DC_slope;

float32_t IA_DC;
float32_t IB_DC;
float32_t IC_DC;
float32_t I_DC;

float32_t IA_DC_slope;
float32_t IB_DC_slope;
float32_t IC_DC_slope;
float32_t I_DC_slope;

float32_t ON_degree_A;
float32_t ON_degree_B;
float32_t ON_degree_C;
float32_t ON_degree_ABC;

float32_t OFF_degree_A;
float32_t OFF_degree_B;
float32_t OFF_degree_C;
float32_t OFF_degree_ABC;


volatile int32_t StartPowerStage;
volatile int32_t Relay_Ctrl;
volatile int32_t OutputMode;

//***********Limit Subsystem**********//


float32_t VA_fundamental_max;
float32_t VA_fundamental_min;
float32_t VB_fundamental_max;
float32_t VB_fundamental_min;
float32_t VC_fundamental_max;
float32_t VC_fundamental_min;
float32_t V_fundamental_max;
float32_t V_fundamental_min;

float32_t IA_fundamental_max;
float32_t IA_fundamental_min;
float32_t IB_fundamental_max;
float32_t IB_fundamental_min;
float32_t IC_fundamental_max;
float32_t IC_fundamental_min;
float32_t I_fundamental_max;
float32_t I_fundamental_min;

float32_t IA_fundamental_max_delay;
float32_t IA_fundamental_min_delay;
float32_t IB_fundamental_max_delay;
float32_t IB_fundamental_min_delay;
float32_t IC_fundamental_max_delay;
float32_t IC_fundamental_min_delay;
float32_t I_fundamental_max_delay;
float32_t I_fundamental_min_delay;

float32_t VA_DC_max;
float32_t VA_DC_min;
float32_t VB_DC_max;
float32_t VB_DC_min;
float32_t VC_DC_max;
float32_t VC_DC_min;
float32_t V_DC_max;
float32_t V_DC_min;

float32_t IA_DC_max;
float32_t IA_DC_min;
float32_t IB_DC_max;
float32_t IB_DC_min;
float32_t IC_DC_max;
float32_t IC_DC_min;
float32_t I_DC_max;
float32_t I_DC_min;

float32_t AC_Freq_Ref_A_max;
float32_t AC_Freq_Ref_A_min;
float32_t AC_Freq_Ref_B_max;
float32_t AC_Freq_Ref_B_min;
float32_t AC_Freq_Ref_C_max;
float32_t AC_Freq_Ref_C_min;
float32_t AC_Freq_Ref_max;
float32_t AC_Freq_Ref_min;

float32_t powerRms_A_Watts_max;
float32_t powerRms_A_Watts_min;
float32_t powerRms_B_Watts_max;
float32_t powerRms_B_Watts_min;
float32_t powerRms_C_Watts_max;
float32_t powerRms_C_Watts_min;
float32_t powerRmsTotal_Watts_max;
float32_t powerRmsTotal_Watts_min;


float32_t OldVA_RMS_meas;
float32_t OldIA_RMS_meas;

//************Measure Subsytem************//

float32_t VA_RMS_meas;
float32_t VB_RMS_meas;
float32_t VC_RMS_meas;
float32_t VAB_RMS_meas;
float32_t VBC_RMS_meas;
float32_t VCA_RMS_meas;
float32_t IA_RMS_meas;
float32_t IB_RMS_meas;
float32_t IC_RMS_meas;
float32_t IAB_RMS_meas;
float32_t IBC_RMS_meas;
float32_t ICA_RMS_meas;

float32_t VA_PEAK_meas;
float32_t VB_PEAK_meas;
float32_t VC_PEAK_meas;

float32_t IA_PEAK_meas;
float32_t IB_PEAK_meas;
float32_t IC_PEAK_meas;


float32_t VA_DC_meas;
float32_t VB_DC_meas;
float32_t VC_DC_meas;
float32_t IA_DC_meas;
float32_t IB_DC_meas;
float32_t IC_DC_meas;

float32_t Freq_Meas_A;
float32_t Freq_Meas_B;
float32_t Freq_Meas_C;

float32_t powerRms_A_Watts;
float32_t powerRms_B_Watts;
float32_t powerRms_C_Watts;
float32_t powerRmsTotal_Watts;

float32_t powerRms_A_VARS;
float32_t powerRms_B_VARS;
float32_t powerRms_C_VARS;
float32_t powerRmsTotal_VARS;

float32_t powerRms_A_VA;
float32_t powerRms_B_VA;
float32_t powerRms_C_VA;
float32_t powerRmsTotal_VA;

float32_t powerFactor_A;
float32_t powerFactor_B;
float32_t powerFactor_C;


//************Measure Subsystem INPUT ************//

float32_t VA_RMS_INPUT_meas;
float32_t VB_RMS_INPUT_meas;
float32_t VC_RMS_INPUT_meas;
float32_t VAB_RMS_INPUT_meas;
float32_t VBC_RMS_INPUT_meas;
float32_t VCA_RMS_INPUT_meas;
float32_t IA_RMS_INPUT_meas;
float32_t IB_RMS_INPUT_meas;
float32_t IC_RMS_INPUT_meas;
float32_t IAB_RMS_INPUT_meas;
float32_t IBC_RMS_INPUT_meas;
float32_t ICA_RMS_INPUT_meas;

float32_t Freq_INPUT_Meas_A;
float32_t Freq_INPUT_Meas_B;
float32_t Freq_INPUT_Meas_C;

float32_t powerRms_A_Input_Watts;
float32_t powerRms_B_Input_Watts;
float32_t powerRms_C_Input_Watts;
float32_t powerRmsTotal_Input_Watts;

float32_t powerRms_A_Input_VARS;
float32_t powerRms_B_Input_VARS;
float32_t powerRms_C_Input_VARS;
float32_t powerRmsTotal_Input_VARS;

float32_t powerRms_A_Input_VA;
float32_t powerRms_B_Input_VA;
float32_t powerRms_C_Input_VA;
float32_t powerRmsTotal_Input_VA;

float32_t powerFactor_INPUT_A;
float32_t powerFactor_INPUT_B;
float32_t powerFactor_INPUT_C;

float32_t Vdc_fb;
float32_t efficiency;

#endif



