/**
 * @file pid_controller.h
 * @author your name (your_email@doman.com), Jared McArthur
 * @brief PID controller struct and relevant functions
 * @version 0.1
 * @date 2022-11-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdint.h>

typedef struct PIDController {
    int32_t kpn;   // numerator of proportional coefficient
    int32_t kpd;   // denominator of proportional coefficient
    int32_t kin;   // numerator of integral coefficient
    int32_t kid;   // denominator of integral coefficient
    int32_t kdn;   // numerator of differential coefficient
    int32_t kdd;   // denominator of differential coefficient
    // TODO: add additional variables (eg. error, desired_speed, actual_speed, etc.)
} pid_controller_t;

/**
 * @brief Initialize a pid_controller_t object
 * 
 * @param kpn numerator of proportional coefficient
 * @param kpd denominator of proportional coefficient
 * @param kin numerator of integral coefficient
 * @param kid denominator of integral coefficient
 * @param kdn numerator of differential coefficient
 * @param kdd denominator of differential coefficient
 */
pid_controller_t pid_controller_init(int32_t kpn, int32_t kpd, int32_t kin, int32_t kid, int32_t kdn, int32_t kdd);

// TODO: add additional functions for executing the logic for the PID controller
