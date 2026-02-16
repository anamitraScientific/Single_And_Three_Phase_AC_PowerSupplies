/*
 * PowerAnalyzer.h
 *
 *  Created on: Oct 3, 2025
 *      Author: admin
 */

#ifndef MY_FILES_HEADERS_POWERANALYZER_H_
#define MY_FILES_HEADERS_POWERANALYZER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "driverlib.h"
#include "device.h"
#include "project_defines.h"

//typedef volatile struct{
//    // config
//    float fs_hz;          // sample rate
//    float zc_hyst;        // zero-cross hysteresis (same units as x)
//    float smooth_tau_cyc; // smoothing time-constant in cycles (e.g., 1.5)
//
//    // state (half-cycle accumulation)
//    float sumsq;          // sum of x^2 within current half-cycle
//    uint32_t n;           // samples in current half-cycle
//    int16_t sign;         // -1, 0, +1 (hysteretic sign)
//    uint16_t half_count;   // 0/1: two halves make a full cycle
//
//    // stash previous half
//    float last_half_mean2; // mean(x^2) of previous half-cycle
//
//    // outputs
//    float32_t rms_cycle;      // per-cycle RMS (updated once per cycle)
//    float32_t rms_smooth;     // IIR-smoothed RMS (updated every sample)
//
//    // internal smoothing coeff
//    float alpha;          // per-sample IIR coefficient
//}RMS_PerCycle;
//
///* Compute alpha for per-sample IIR given tau in cycles */
//static inline float _rms_alpha_from_tau(float fs_hz, float tau_cyc, float freq_hz)
//{
//    // time constant in seconds ~ tau_cyc / freq
//    float tau_s = (freq_hz > 1e-3f) ? (tau_cyc / freq_hz) : 0.05f; // fallback 50 ms
//    float Ts = 1.0f / fs_hz;
//    float a = 1.0f - expf(-Ts / fmaxf(tau_s, 1e-3f));
//    if (a < 1e-5f) a = 1e-5f;
//    if (a > 0.2f)  a = 0.2f;   // keep smoothing stable
//    return a;
//}
//
//static inline void RMS_PerCycle_init(RMS_PerCycle *s,
//                                     float fs_hz,
//                                     float zc_hyst,
//                                     float smooth_tau_cyc /* ~1.0–2.0 cycles */)
//{
//    s->fs_hz         = fs_hz;
//    s->zc_hyst       = (zc_hyst >= 0.0f) ? zc_hyst : 0.0f;
//    s->smooth_tau_cyc= (smooth_tau_cyc > 0.0f) ? smooth_tau_cyc : 1.5f;
//
//    s->sumsq = 0.0f;
//    s->n = 0;
//    s->sign = 0;
//    s->half_count = 0;
//    s->last_half_mean2 = 0.0f;
//
//    s->rms_cycle = 0.0f;
//    s->rms_smooth = 0.0f;
//
//    // Start with a reasonable default alpha (~50 ms at 50 Hz)
//    s->alpha = _rms_alpha_from_tau(fs_hz, s->smooth_tau_cyc, 50.0f);
//}
//
///* Optional: call this when you have a current frequency estimate (Hz)
//   to keep smoothing roughly "one to two cycles" across 50–500 Hz. */
//static inline void RMS_PerCycle_update_alpha_for_freq(RMS_PerCycle *s, float freq_hz)
//{
//    s->alpha = _rms_alpha_from_tau(s->fs_hz, s->smooth_tau_cyc, freq_hz);
//}
//
//
///* Feed one instantaneous, already re-scaled bipolar sample (e.g., volts) */
//static inline float RMS_PerCycle_update(RMS_PerCycle *s, float adcVal)
//{
//    // Hysteretic sign detection
//    int16_t new_sign = s->sign;
//    if (adcVal >  s->zc_hyst) new_sign = +1;
//    else if (adcVal < -s->zc_hyst) new_sign = -1;
//
//    // Accumulate current sample
//    s->sumsq += adcVal * adcVal;
//    s->n++;
//
//    // Check half-cycle boundary (sign flip from nonzero -> opposite)
//    if ((s->sign != 0) && (new_sign != s->sign)) {
//        // Finish this half
//        float half_mean2 = (s->n > 0) ? (s->sumsq / (float)s->n) : 0.0f;
//
//        if (s->half_count == 0) {
//            // first half stored
//            s->last_half_mean2 = half_mean2;
//            s->half_count = 1;
//        } else {
//            // second half -> combine to full-cycle RMS
//            float full_mean2 = 0.5f * (s->last_half_mean2 + half_mean2);
//            float rms = sqrtf(fmaxf(full_mean2, 0.0f));
//            s->rms_cycle = rms;
//
//            // Update smoothed RMS with per-sample IIR
//            if (s->rms_smooth <= 0.0f) s->rms_smooth = rms; // initialize
//            s->rms_smooth += s->alpha * (rms - s->rms_smooth);
//
//            s->half_count = 0;
//        }
//
//        // reset accumulators for next half
//        s->sumsq = 0.0f;
//        s->n = 0;
//    } else {
//        // keep smoothing running each sample toward last known cycle RMS (optional)
//        // This keeps display stable between cycle-completes
//        s->rms_smooth += s->alpha * (s->rms_cycle - s->rms_smooth);
//    }
//
//    s->sign = new_sign;
//    return s->rms_smooth; // convenient live value
//}

typedef volatile struct{
    int count;
    float32_t sum;
    float32_t rmsVal;
}RMS_Cycle;

typedef struct {
    /* config */
    float32_t fs_hz;       // sample rate (e.g., 50000.0f)
    float32_t hyst_abs;    // fixed hysteresis in signal units (set 0 to auto)
    float32_t hyst_k;      // auto-hysteresis = hyst_k * avg_abs (e.g., 0.02f)
    float32_t ema_alpha;   // smoothing for freq (0..1), e.g. 0.2f

    /* state */
    float32_t avg_abs;     // running |x| for auto hysteresis
    float32_t prev_x;      // previous sample
    int16_t   state;       // Schmitt state: -1, 0, +1
    float32_t last_cross;  // last crossing index (fractional)
    float32_t idx;         // running sample index (fractional-safe)
    float32_t freq_hz;     // smoothed frequency estimate
} FREQ_ZC;

extern void RMS_Cycle_init(RMS_Cycle *s);
extern float32_t RMS_Cycle_Update(RMS_Cycle *s, float32_t adcVal, float32_t freq);

extern void FREQ_ZC_init(FREQ_ZC *s, float32_t fs_hz,
                         float32_t hyst_abs, float32_t hyst_k,
                         float32_t ema_alpha);

extern float32_t FREQ_ZC_update(FREQ_ZC *s, float32_t x);

#endif /* MY_FILES_HEADERS_POWERANALYZER_H_ */
