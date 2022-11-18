/**
 * @file        lab10.c
 * @author      your name (your_email@doman.com), Jonathan Valvano, Matthew Yu, Jared McArthur
 *              <TA NAME and LAB SECTION # HERE>
 * @brief       An empty main file for running lab10.
 * @version     0.1.0
 * @date        2022-11-17 <REPLACE WITH DATE OF LAST REVISION>
 * 
 * @copyright   Copyright (c) 2022
 */

#include <stdint.h>

#include "inc/tm4c123gh6pm.h"
#include "inc/PLL.h"
#include "inc/CortexM.h"
#include "inc/Unified_Port_Init.h"

/* Add whatever else you need here! */
#include "lib/pid_controller/pid_controller.h"
#include "lib/pid_controller/pid_controller_parser.h"

int main(void) {
    DisableInterrupts();
    PLL_Init(Bus80MHz);

    // initialize things

    // initialize pid controller
    pid_controller_t pid_controller = pid_controller_init(0, 0, 0, 0, 0, 0);

    EnableInterrupts();

    // start pid controller parser (never returns)
    pid_controller_parser_start(&pid_controller);

    // should never reach
    return 1;
}
