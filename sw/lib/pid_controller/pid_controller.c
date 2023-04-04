/**
 * @file pid_controller_parser.c
 * @author Jared McArthur
 * @brief 
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "lib/pid_controller/pid_controller.h"

pid_controller_t pid_controller_init(int32_t kpn, int32_t kpd, int32_t kin, int32_t kid, int32_t kdn, int32_t kdd, int32_t des_spd) {
    pid_controller_t pid_controller = {
        .kpn = kpn,
        .kpd = kpd,
        .kin = kin,
        .kid = kid,
        .kdn = kdn,
        .kdd = kdd,
        .des_spd = des_spd,
        .act_spd = 0,
        .p   = 0,
        .i   = 0,
        .d   = 0,
        .u   = 0,
        .err = 0
    };

    // TODO: initialize timer(s) to change duty cycle, get error, etc.

    return pid_controller;
}