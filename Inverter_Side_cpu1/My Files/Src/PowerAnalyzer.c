/*
 * PowerAnalyzer.c
 *
 *  Created on: Oct 3, 2025
 *      Author: Anamitra
 */

#include "PowerAnalyzer.h"


void RMS_Cycle_init(RMS_Cycle *s)
{
//    sum = 0;
//    count = 0;
//    rmsVal = 0;
    s->sum = 0;
    s->count = 0;
    s->rmsVal = 0;
}

float32_t RMS_Cycle_Update(RMS_Cycle *s, float32_t adcVal, float32_t freq)
{
    int N;
    N = 40000/freq;
    if(s->count <N){
        s->sum = s->sum + (adcVal * adcVal);
        s->count++;
    }
    else if(s->count == N){
//        s->rmsVal = sqrtf(s->sum/N);
        EMAVG_MACRO(sqrtf(s->sum/N), s->rmsVal, 0.9);
        s->count = 0;
        s->sum = 0;
    }
    return s->rmsVal;
}


void FREQ_ZC_init(FREQ_ZC *s, float32_t fs_hz,
                                float32_t hyst_abs, float32_t hyst_k,
                                float32_t ema_alpha)
{
    s->fs_hz     = fs_hz;
    s->hyst_abs  = hyst_abs;     // e.g., 0.0f for auto
    s->hyst_k    = hyst_k;       // e.g., 0.02f (=approx 2% of amplitude)
    s->ema_alpha = ema_alpha;    // e.g., 0.2f

    s->avg_abs   = 0.0f;
    s->prev_x    = 0.0f;
    s->state     = 0;
    s->last_cross= -1.0f;        // "no crossing yet"
    s->idx       = 0.0f;
    s->freq_hz   = 0.0f;
}


float32_t FREQ_ZC_update(FREQ_ZC *s, float32_t x)
{
    /* ----- auto hysteresis based on running |x| (optional) ----- */
    // slow EMA for |x| to avoid reacting to spikes
    const float32_t k_abs = 0.01f;                 // ~100-sample time constant
    s->avg_abs += k_abs * (fabsf(x) - s->avg_abs);

    const float32_t th_auto = s->hyst_k * s->avg_abs;
    const float32_t th_fixed = s->hyst_abs;
    float32_t th = (th_fixed > 0.0f) ? th_fixed : th_auto;
    if (th < 1e-6f) th = 1e-6f;                    // never zero

    /* ----- Schmitt state ----- */
    int16_t new_state = s->state;
    if (x >  th) new_state = +1;
    else if (x < -th) new_state = -1;
    // else keep previous state

    /* ----- Detect rising threshold crossing and interpolate crossing time ----- */
    if (s->state < 0 && new_state > 0) {
        // Threshold crossing occurred between (idx-1) and (idx) at level +th
        float32_t x0 = s->prev_x;
        float32_t x1 = x;
        float32_t denom = (x1 - x0);
        float32_t frac = 0.5f;                    // fallback
        if (fabsf(denom) > 1e-12f) {
            frac = (th - x0) / denom;             // fraction into current sample
            if (frac < 0.0f) frac = 0.0f;
            if (frac > 1.0f) frac = 1.0f;
        }
        float32_t cross_idx = (s->idx - 1.0f) + frac;

        if (s->last_cross > 0.0f) {
            float32_t n_samp = cross_idx - s->last_cross;
            if (n_samp > 1.0f) {
                float32_t f_now = s->fs_hz / n_samp;

                /* clamp to sane range (your spec: 50..500 Hz) */
                if (f_now < 40.0f)  f_now = 40.0f;
                if (f_now > 25000.0f) f_now = 25000.0f;

                /* smooth */
                if (s->freq_hz <= 0.0f) s->freq_hz = f_now;  // first lock
                s->freq_hz += s->ema_alpha * (f_now - s->freq_hz);
            }
        }
        s->last_cross = cross_idx;
    }

    /* advance state */
    s->prev_x = x;
    s->state  = new_state;
    s->idx   += 1.0f;

    return s->freq_hz;
}
















