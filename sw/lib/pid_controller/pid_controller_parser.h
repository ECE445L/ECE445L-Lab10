/**
 * @file parser.h
 * @author your name (your_email@doman.com), Jared McArthur
 * @brief Serial input parser for the PID controller
 * @version 0.1
 * @date 2022-11-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "lib/pid_controller/pid_controller.h"

/**
 * @brief Starts PID controller UART parser on PA1-0. Spins infinitely so do NOT place in an ISR.
 * 
 * @param pid_controller 
 */
void pid_controller_parser_start(pid_controller_t* pid_controller);
