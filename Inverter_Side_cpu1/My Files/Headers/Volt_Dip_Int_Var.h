/*
 * Volt_Dip_Int_Var.h
 *
 *  Created on: Feb 17, 2026
 *      Author: Anamitra Sarkar
 *
 *      Note: This file includes sequences to generate
 *      Voltage dips, interruptions & variations
 *      as per EMC standard IEC 61000-4-11:2020
 */

#ifndef MY_FILES_HEADERS_VOLT_DIP_INT_VAR_H_
#define MY_FILES_HEADERS_VOLT_DIP_INT_VAR_H_

#include "NPC_Solution.h"

extern float32_t pll_ang_A_prev;
extern float32_t ratedVoltage;
extern float32_t reducedVolatge;
extern float32_t isrInterruptPerCycle;
extern float32_t InterruptCount;
extern float32_t restoringSlew;

extern void RUN_INV_ISR_DipsAndInterruptionSequence(void);

#endif /* MY_FILES_HEADERS_VOLT_DIP_INT_VAR_H_ */
