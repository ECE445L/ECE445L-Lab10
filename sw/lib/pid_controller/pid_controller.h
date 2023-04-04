/**
 * @file pid_controller.h
 * @author Jared McArthur
 * @brief Parser for the PID python interface.
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdint.h>

typedef struct PIDController {
    int32_t kpn;        // numerator of proportional coefficient
    int32_t kpd;        // denominator of proportional coefficient
    int32_t kin;        // numerator of integral coefficient
    int32_t kid;        // denominator of integral coefficient
    int32_t kdn;        // numerator of differential coefficient
    int32_t kdd;        // denominator of differential coefficient
    int32_t des_spd;    // desired speed of the motor
    int32_t act_spd;    // actual speed of the motor
    int32_t p;          // proportional component
    int32_t i;          // integral component
    int32_t d;          // derivative component
    int32_t u;          // control value
    int32_t err;        // error value
    // TODO: add additional variables as necessary
} pid_controller_t;

/**
 * @brief Initialize a pid_controller object
 * 
 * @param kpn       numerator of proportional coefficient
 * @param kpd       denominator of proportional coefficient
 * @param kin       numerator of integral coefficient
 * @param kid       denominator of integral coefficient
 * @param kdn       numerator of differential coefficient
 * @param kdd       denominator of differential coefficient
 * @param des_spd   desired speed of the motor
 * @return pid_controller_t 
 */
pid_controller_t pid_controller_init(int32_t kpn, int32_t kpd, int32_t kin, int32_t kid, int32_t kdn, int32_t kdd, int32_t des_spd);

// TODO: add additional functions as necessary