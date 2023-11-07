/**
 * @file pid_controller_parser.c
 * @author your name (your_email@doman.com), Jared McArthur
 * @brief PID controller struct and relevant functions
 * @version 0.1
 * @date 2022-11-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "lib/pid_controller/pid_controller.h"

pid_controller_t pid_controller_init(int32_t kpn, int32_t kpd, int32_t kin, int32_t kid, int32_t kdn, int32_t kdd) {
    // prevent 0s in denominators
    kpd = (!kpd * 1) + kpd;
    kid = (!kid * 1) + kid;
    kdd = (!kdd * 1) + kdd;

    // convert negative numbers into positive
    kpn = (((((kpn & 0x80000000) >> 31) * -1) ^ kpn) + ((kpn & 0x80000000) >> 31)) & ~0x80000000;
    kpd = (((((kpd & 0x80000000) >> 31) * -1) ^ kpd) + ((kpd & 0x80000000) >> 31)) & ~0x80000000;
    kin = (((((kin & 0x80000000) >> 31) * -1) ^ kin) + ((kin & 0x80000000) >> 31)) & ~0x80000000;
    kid = (((((kid & 0x80000000) >> 31) * -1) ^ kid) + ((kid & 0x80000000) >> 31)) & ~0x80000000;
    kdn = (((((kpn & 0x80000000) >> 31) * -1) ^ kdn) + ((kdn & 0x80000000) >> 31)) & ~0x80000000;
    kdd = (((((kdd & 0x80000000) >> 31) * -1) ^ kdd) + ((kdd & 0x80000000) >> 31)) & ~0x80000000;

    pid_controller_t pid_controller = {
        .kpn = kpn,
        .kpd = kpd,
        .kin = kin,
        .kid = kid,
        .kdn = kdn,
        .kdd = kdd
    };

    // TODO: initialize timer(s) and input capture(s) to view and edit speed, error, etc.

    return pid_controller;
}

// TODO: add additional functions for executing the logic for the PID controller
