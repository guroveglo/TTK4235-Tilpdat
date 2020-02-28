/**
 * @file
 * @brief Implements the logic of the elevator. 
 *
 *
 * Veglo, Lønvik
 */
#ifndef LOGIC_H
#define LOGIC_H
#define TIME_DELAY 3.0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hardware.h"
#include "order.h" 
#include "door.h"

/**
 * @param stop_pressed 1 if stop button is pressed.
 */
extern int stop_pressed; 

/**
 * @brief Resets elevator panel and drives the elevator to initial condition. [1st floor].
 */
void start_condition();

/**
 * @brief Checks what floor the elevator is on.
 *
 * @return Integer of the floor. If between floors, return -1.
 */
int read_floor();

/**
 * @brief Turns on floor indicator light.
 */
void floor_indicator();

/**
 * @brief Resets the lights on the elevator panel.
 */
void reset_lights();

/**
 * @brief Checks if the stop button is pressed. If pressed, delete all orders and update @p stop_pressed.
 */
void check_and_stop_elevator();
/**
 * @brief Time delay while the door is open. Extends the delay if the stop button is pressed or active obstruction signal. 
 */
void door_delay();

#endif