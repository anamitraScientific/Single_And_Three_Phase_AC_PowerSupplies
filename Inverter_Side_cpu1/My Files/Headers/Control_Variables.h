/*
 * Control_Variables.h
 *
 *  Created on: Jul 21, 2025
 *      Author: Anamitra
 */

#ifndef CONTROL_VARIABLES_H_
#define CONTROL_VARIABLES_H_

#include "driverlib.h"
#include "device.h"
#include "project_defines.h"
#include <stdbool.h>



typedef union
{
    float32_t fdata;
    uint32_t u32data;
}DataConverter;



//**********************************************//

#if CONVERTER_TYPE == SINGLE_PHASE

extern float32_t Vac_fundamental;
extern float32_t V_DC;
extern float32_t AC_Freq_Ref;
extern float32_t slope_VacRef;
extern float32_t V_DC_slope;
extern float32_t slope_FreqRef;
extern volatile int32_t StartPowerStage;
extern volatile int32_t StartPowerStage_prev;
extern float32_t ON_degree;
extern float32_t OFF_degree;
extern volatile int32_t CouplingMode;

extern float32_t Limit_VAC;
extern float32_t Limit_VDC_max;
extern float32_t Limit_VDC_min;
extern float32_t Limit_AC_Freq_Ref;
extern float32_t Limit_OPP;
extern float32_t Limit_OCP;
extern float32_t Limit_OCP_Delay;

/* MEASUREMENT VARIABLES */
extern float32_t Meas_Vrms;            // RMS Voltage
extern float32_t Meas_Vdc;             // DC Voltage component
extern float32_t Meas_Vac;             // AC Voltage component
extern float32_t Meas_Idc;             // DC Current component
extern float32_t Meas_Irms;            // RMS Current
extern float32_t Meas_Iac;             // AC Current component
extern float32_t Meas_Freq;            // Measured Frequency

extern float32_t Meas_Vpk_P;           // Peak Voltage
extern float32_t Meas_Ipk_P;           // Peak Current

extern float32_t Meas_Vpk_N;           // Peak Voltage
extern float32_t Meas_Ipk_N;           // Peak Current

extern float32_t Meas_V_CF;            // Crest Factor
extern float32_t Meas_I_CF;            // Crest Factor

extern float32_t Meas_Is;              // Surge Current / Inrush Current

extern float32_t Meas_Preal;           // Real Power (Watts)
extern float32_t Meas_Preactive;       // Reactive Power (VAR)
extern float32_t Meas_Papparent;       // Apparent Power (VA)
extern float32_t Meas_PF;              // Power Factor

//extern float32_t Vdc_fb;

#elif CONVERTER_TYPE == THREE_PHASE

//*********** Source Subsystem ***********//

extern float32_t VA_fundamental;
extern float32_t VB_fundamental;
extern float32_t VC_fundamental;
extern float32_t Vac_fundamental;

extern float32_t slope_VaRef;
extern float32_t slope_VbRef;
extern float32_t slope_VcRef;
extern float32_t slope_VacRef;

extern float32_t IA_fundamental;
extern float32_t IB_fundamental;
extern float32_t IC_fundamental;
extern float32_t Iac_fundamental;

extern float32_t slope_IaRef;
extern float32_t slope_IbRef;
extern float32_t slope_IcRef;
extern float32_t slope_IacRef;

extern float32_t AC_Freq_Ref_A;
extern float32_t AC_Freq_Ref_B;
extern float32_t AC_Freq_Ref_C;
extern float32_t AC_Freq_Ref;

extern float32_t slope_FreqRef;
extern float32_t slope_FreqRefA;
extern float32_t slope_FreqRefB;
extern float32_t slope_FreqRefC;

extern float32_t Phase_angle_AB;
extern float32_t Phase_angle_AC;
extern float32_t Phase_angle_BC;

extern float32_t Phase_seq_ABC;
extern float32_t Phase_seq_ACB;

extern float32_t VA_DC;
extern float32_t VB_DC;
extern float32_t VC_DC;
extern float32_t V_DC;

extern float32_t VA_DC_slope;
extern float32_t VB_DC_slope;
extern float32_t VC_DC_slope;
extern float32_t V_DC_slope;

extern float32_t IA_DC;
extern float32_t IB_DC;
extern float32_t IC_DC;
extern float32_t I_DC;

extern float32_t IA_DC_slope;
extern float32_t IB_DC_slope;
extern float32_t IC_DC_slope;
extern float32_t I_DC_slope;

extern float32_t ON_degree_A;
extern float32_t ON_degree_B;
extern float32_t ON_degree_C;
extern float32_t ON_degree_ABC;

extern float32_t OFF_degree_A;
extern float32_t OFF_degree_B;
extern float32_t OFF_degree_C;
extern float32_t OFF_degree_ABC;

extern volatile int32_t StartPowerStage;

extern volatile int32_t StartPowerStage_temp;
extern volatile int32_t StartPowerStage_prev;


extern volatile int32_t Relay_Ctrl;
extern volatile int32_t OutputMode;

//*********** Limit Subsystem ***********//

extern float32_t VA_fundamental_max;
extern float32_t VA_fundamental_min;
extern float32_t VB_fundamental_max;
extern float32_t VB_fundamental_min;
extern float32_t VC_fundamental_max;
extern float32_t VC_fundamental_min;
extern float32_t V_fundamental_max;
extern float32_t V_fundamental_min;

extern float32_t IA_fundamental_max;
extern float32_t IA_fundamental_min;
extern float32_t IB_fundamental_max;
extern float32_t IB_fundamental_min;
extern float32_t IC_fundamental_max;
extern float32_t IC_fundamental_min;
extern float32_t I_fundamental_max;
extern float32_t I_fundamental_min;

extern float32_t IA_fundamental_max_delay;
extern float32_t IA_fundamental_min_delay;
extern float32_t IB_fundamental_max_delay;
extern float32_t IB_fundamental_min_delay;
extern float32_t IC_fundamental_max_delay;
extern float32_t IC_fundamental_min_delay;
extern float32_t I_fundamental_max_delay;
extern float32_t I_fundamental_min_delay;

extern float32_t VA_DC_max;
extern float32_t VA_DC_min;
extern float32_t VB_DC_max;
extern float32_t VB_DC_min;
extern float32_t VC_DC_max;
extern float32_t VC_DC_min;
extern float32_t V_DC_max;
extern float32_t V_DC_min;

extern float32_t IA_DC_max;
extern float32_t IA_DC_min;
extern float32_t IB_DC_max;
extern float32_t IB_DC_min;
extern float32_t IC_DC_max;
extern float32_t IC_DC_min;
extern float32_t I_DC_max;
extern float32_t I_DC_min;

extern float32_t AC_Freq_Ref_A_max;
extern float32_t AC_Freq_Ref_A_min;
extern float32_t AC_Freq_Ref_B_max;
extern float32_t AC_Freq_Ref_B_min;
extern float32_t AC_Freq_Ref_C_max;
extern float32_t AC_Freq_Ref_C_min;
extern float32_t AC_Freq_Ref_max;
extern float32_t AC_Freq_Ref_min;

extern float32_t powerRms_A_Watts_max;
extern float32_t powerRms_A_Watts_min;
extern float32_t powerRms_B_Watts_max;
extern float32_t powerRms_B_Watts_min;
extern float32_t powerRms_C_Watts_max;
extern float32_t powerRms_C_Watts_min;
extern float32_t powerRmsTotal_Watts_max;
extern float32_t powerRmsTotal_Watts_min;

//************ Measure Subsystem ************//
extern float32_t VA_RMS_meas;
extern float32_t VB_RMS_meas;
extern float32_t VC_RMS_meas;
extern float32_t VAB_RMS_meas;
extern float32_t VBC_RMS_meas;
extern float32_t VCA_RMS_meas;
extern float32_t IA_RMS_meas;
extern float32_t IB_RMS_meas;
extern float32_t IC_RMS_meas;
extern float32_t IAB_RMS_meas;
extern float32_t IBC_RMS_meas;
extern float32_t ICA_RMS_meas;

extern float32_t VA_PEAK_meas;
extern float32_t VB_PEAK_meas;
extern float32_t VC_PEAK_meas;

extern float32_t IA_PEAK_meas;
extern float32_t IB_PEAK_meas;
extern float32_t IC_PEAK_meas;

extern float32_t VA_DC_meas;
extern float32_t VB_DC_meas;
extern float32_t VC_DC_meas;
extern float32_t IA_DC_meas;
extern float32_t IB_DC_meas;
extern float32_t IC_DC_meas;

extern float32_t Freq_Meas_A;
extern float32_t Freq_Meas_B;
extern float32_t Freq_Meas_C;

extern float32_t powerRms_A_Watts;
extern float32_t powerRms_B_Watts;
extern float32_t powerRms_C_Watts;
extern float32_t powerRmsTotal_Watts;

extern float32_t powerRms_A_VARS;
extern float32_t powerRms_B_VARS;
extern float32_t powerRms_C_VARS;
extern float32_t powerRmsTotal_VARS;

extern float32_t powerRms_A_VA;
extern float32_t powerRms_B_VA;
extern float32_t powerRms_C_VA;
extern float32_t powerRmsTotal_VA;

extern float32_t powerFactor_A;
extern float32_t powerFactor_B;
extern float32_t powerFactor_C;

//************ Measure Subsystem INPUT ************//
extern float32_t VA_RMS_INPUT_meas;
extern float32_t VB_RMS_INPUT_meas;
extern float32_t VC_RMS_INPUT_meas;
extern float32_t VAB_RMS_INPUT_meas;
extern float32_t VBC_RMS_INPUT_meas;
extern float32_t VCA_RMS_INPUT_meas;
extern float32_t IA_RMS_INPUT_meas;
extern float32_t IB_RMS_INPUT_meas;
extern float32_t IC_RMS_INPUT_meas;
extern float32_t IAB_RMS_INPUT_meas;
extern float32_t IBC_RMS_INPUT_meas;
extern float32_t ICA_RMS_INPUT_meas;

extern float32_t Freq_INPUT_Meas_A;
extern float32_t Freq_INPUT_Meas_B;
extern float32_t Freq_INPUT_Meas_C;

extern float32_t powerRms_A_Input_Watts;
extern float32_t powerRms_B_Input_Watts;
extern float32_t powerRms_C_Input_Watts;
extern float32_t powerRmsTotal_Input_Watts;

extern float32_t powerRms_A_Input_VARS;
extern float32_t powerRms_B_Input_VARS;
extern float32_t powerRms_C_Input_VARS;
extern float32_t powerRmsTotal_Input_VARS;

extern float32_t powerRms_A_Input_VA;
extern float32_t powerRms_B_Input_VA;
extern float32_t powerRms_C_Input_VA;
extern float32_t powerRmsTotal_Input_VA;

extern float32_t powerFactor_INPUT_A;
extern float32_t powerFactor_INPUT_B;
extern float32_t powerFactor_INPUT_C;

extern float32_t Vdc_fb;
extern float32_t efficiency;

#endif

extern bool seq_start;
extern float32_t seq_table[100][3];

#endif /* CONTROL_VARIABLES_H_ */
