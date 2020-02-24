/**
 * @file
 * @brief Implements the logic of the elevator. 
 *
 *
 * Veglo, Lønvik
 */
#ifndef LOGIC
#define LOGIC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hardware.h"
#include "order.h"
#include "light.h"
#include "door.h"

/**
 * @param stop_pressed 1 if stop button is pressed.
 */
int stop_pressed;


/**
 * @brief Set's the elevator to initial condition. [1st floor]
 */
void start_condition();

/**
 * @brief Checks which the elevator is at.
 *
 * @return the integer of the floor. If between floors, return -1.
 */
int read_floor();

/**
 * @brief Resets the lights of the elevator panel.
 */
void reset_lights();

/**
 * @brief Checks if stop_button is pressed. If pressed, delete all orders.
 */
void check_and_stop_elevator();
/**
 * @brief Time delay caused by stop button, open door and obstruction signal. 
 */
void timer();

#endif