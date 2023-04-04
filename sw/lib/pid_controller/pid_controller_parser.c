/**
 * @file pid_controller_parser.c
 * @author Jared McArthur
 * @brief Serial input parser for the PID controller
 * @version 0.1
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "inc/UART.h"

#include "lib/pid_controller/pid_controller_parser.h"

#define MAX_CMD_SIZE    20      // max size for a parser command

// uart buffer to store characters
static char uart_buffer[MAX_CMD_SIZE + 2] = {0};
static uint32_t uart_buffer_index = 0;
static char cmd_buffer[MAX_CMD_SIZE + 2] = {0};
static char param_buffer[MAX_CMD_SIZE + 2] = {0};

bool str_equals(char* a, char* b) {
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            return false;
        }
        ++a;
        ++b;
    }
    if (*a != *b) {
        return false;
    }
    return true;
}

uint32_t str_to_uint(char* str) {
    uint32_t n = 0;
    uint32_t count = 0;
    while(*str != 0) {
        ++count;
        ++str;
    }
    --str;
    for(uint32_t i = 0, j = 1; i < count; ++i, --str) {
        if (*str >= '0' && *str <= '9') {
            n += (*str - '0') * j;
            j *= 10;
        }
    }
    return n;
}

void pid_controller_parser_start(pid_controller_t* pid_controller) {
    // initialize UART on PA1-0
    Output_Init();              // initialize output device

    // output the usage of the parser
    printf(
        "Motor Controller Parser:\n"
        "  usage: [help, h] cmd [param]\n"
        "  cmd: which coefficient to change\n"
        "    kpn   - set the kpn\n"
        "    kpd   - set the kpd\n"
        "    kin   - set the kin\n"
        "    kid   - set the kid\n"
        "    kdn   - set the kdn\n"
        "    kdd   - set the kdd\n"
        "    spd   - set the desired speed\n"
        "    stats - output pid controller stats\n"
        "    coeff - output current coefficients\n"
        "  param: unsigned integer to set the coefficient to\n"
    );
    
    // spin and wait for UART inputs
    char character;
    while (1) {
        printf("Enter command...\n");
        do {
            // get a character from the input
            character = UART_InChar();
            // delete an input character
            if (character == DEL && uart_buffer_index) {
                --uart_buffer_index;
                UART_OutChar(character);
            }
            // add a character to buffer
            else if (uart_buffer_index < MAX_CMD_SIZE) {
                uart_buffer[uart_buffer_index++] = character;
                UART_OutChar(character);
            }
        } while (character != CR);
        // null terminate the command
        uart_buffer[--uart_buffer_index] = '\0';
        uart_buffer_index = 0;
        printf("\n");

        // parse the command
        uint32_t i = 0;
        uint32_t cmd_index = 0;
        uint32_t param_index = 0;
        // overlook leading whitespace
        // 0x09, 0x0A, 0x0C, 0x0D, 0x20
        while (uart_buffer[i] == 0x09 || uart_buffer[i] == 0x0A || uart_buffer[i] == 0x0C || uart_buffer[i] == 0x0D || uart_buffer[i] == 0x20 || uart_buffer[i] == '\0') {
            ++i;
        }
        // fill cmd
        while (uart_buffer[i] != 0x09 && uart_buffer[i] != 0x0A && uart_buffer[i] != 0x0C && uart_buffer[i] != 0x0D && uart_buffer[i] != 0x20 && uart_buffer[i] != '\0') {
            cmd_buffer[cmd_index++] = uart_buffer[i++];
        }
        // null terminate cmd
        cmd_buffer[cmd_index] = '\0';
        // overlook whitespace between cmd and param
        while (uart_buffer[i] == 0x09 || uart_buffer[i] == 0x0A || uart_buffer[i] == 0x0C || uart_buffer[i] == 0x0D || uart_buffer[i] == 0x20 || uart_buffer[i] == '\0') {
            ++i;
        }
        // fill param
        while (uart_buffer[i] != 0x09 && uart_buffer[i] != 0x0A && uart_buffer[i] != 0x0C && uart_buffer[i] != 0x0D && uart_buffer[i] != 0x20 && uart_buffer[i] != '\0') {
            param_buffer[param_index++] = uart_buffer[i++];
        }
        // null terminate param
        param_buffer[param_index] = '\0';

        // turn param into an integer
        uint32_t param = str_to_uint(param_buffer);
        
        // execute cmd accordingly
        // set kpn
        if (str_equals(cmd_buffer, "kpn")) {
            pid_controller->kpn = param;
            printf("kpn set to: %d\n", param);
        }
        // set kpd
        else if (str_equals(cmd_buffer, "kpd")) {
            pid_controller->kpd = param;
            printf("kpd set to: %d\n", param);
        }
        // set kin
        else if (str_equals(cmd_buffer, "kin")) {
            pid_controller->kin = param;
            printf("kin set to: %d\n", param);
        }
        // set kid
        else if (str_equals(cmd_buffer, "kid")) {
            pid_controller->kid = param;
            printf("kid set to: %d\n", param);
        }
        // set kdn
        else if (str_equals(cmd_buffer, "kdn")) {
            pid_controller->kdn = param;
            printf("kdn set to: %d\n", param);
        }
        // set kdd
        else if (str_equals(cmd_buffer, "kdd")) {
            pid_controller->kdd = param;
            printf("kdd set to: %d\n", param);
        }
        // set spd
        else if (str_equals(cmd_buffer, "spd")) {
            pid_controller->des_spd = param;
            printf("spd set to: %d\n", param);
        }
        else if (str_equals(cmd_buffer, "stats")) {
            printf(
                "Motor Controller Stats:\n"
                "  desired speed - %d\n"
                "  actual speed  - %d\n"
                "  p             - %d\n"
                "  i             - %d\n"
                "  d             - %d\n"
                "  u             - %d\n"
                "  err           - %d\n",
                pid_controller->des_spd,
                pid_controller->act_spd,
                pid_controller->p,
                pid_controller->i,
                pid_controller->d,
                pid_controller->u,
                pid_controller->err
            );
        }
        // output coefficients
        // Current PID Controller Coefficients:
        //   kpn - [kpn]
        //   kpd - [kpd]
        //   kin - [kin]
        //   kid - [kid]
        //   kdn - [kdn]
        //   kdd - [kdd]
        else if (str_equals(cmd_buffer, "coeff")) {
            printf(
                "Motor Controller Coefficients:\n"
                "  kpn - %d\n"
                "  kpd - %d\n"
                "  kin - %d\n"
                "  kid - %d\n"
                "  kdn - %d\n"
                "  kdd - %d\n",
                pid_controller->kpn,
                pid_controller->kpd,
                pid_controller->kin,
                pid_controller->kid,
                pid_controller->kdn,
                pid_controller->kdd
            );
        }
        // print help statement
        // Motor Controller Parser Help:
        //   usage: [help, h] cmd param
        //   cmd: which coefficient to change
        //     kpn - set the kpn
        //     kpd - set the kpd
        //     kin - set the kin
        //     kid - set the kid
        //     kdn - set the kdn
        //     kdd - set the kdd
        //     spd - set the desired speed
        //     get - output pid controller status
        //   param: unsigned integer to set the coefficient to
        else if (str_equals(cmd_buffer, "help") || str_equals(cmd_buffer, "h")) {
            printf(
                "Motor Controller Parser:\n"
                "  usage: [help, h] cmd [param]\n"
                "  cmd: which coefficient to change\n"
                "    kpn     - set the kpn\n"
                "    kpd     - set the kpd\n"
                "    kin     - set the kin\n"
                "    kid     - set the kid\n"
                "    kdn     - set the kdn\n"
                "    kdd     - set the kdd\n"
                "    spd     - set the desired speed\n"
                "    stats   - output pid controller stats\n"
                "    coeff   - output current coefficients\n"
                "  param: unsigned integer to set the coefficient to\n"
            );
        }
        // invalid command
        else {
            printf("Invalid command...\n");
        }
    }
}