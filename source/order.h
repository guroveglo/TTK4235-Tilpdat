/**
 * @file
 * @brief Order handling for the elevator.
 *
 *
 * Veglo, Lønvik
 */
#ifndef ORDERHANDLING
#define ORDERHANDLING

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"

/**
 * @param dir Current movement.
 * @param save_dir Saves last movement when in stopstate.
 */

HardwareMovement dir;
HardwareMovement save_dir;

/**
 * @brief Sets bits in elevator queue's. [queue_up, queue_down, queue_inside]
 * Turns on/off command order lights. 
 * @param floor Inquired floor.
 * @param order_type 
 * @param set_or_del Add or remove bit from elevator que's.
 */
void elev_queue(int floor, HardwareOrder order_type, int set_or_del);

/**
 * @brief Checks if buttons are pressed. 
 */
void check_buttons();

/**
 * @brief 
 */
void stop_elev_open_door();

/**
 * @brief Take care of ordered floor in the same direction as @param dir. 
 * @return 1 if requested floor is in the same direction. 
 */
int move_same_dir();

/**
 * @brief Starts the elevator if buttons pressed and elevator in stop state. Will change direction if no orders in same direction.
 */
void from_stop_to_run();

/**
 * @brief Take care of order in opposite direction.
 */
int change_dir();


/**
 * @brief Main controlsystem.
 * 
 */
void order_handling();

#endif