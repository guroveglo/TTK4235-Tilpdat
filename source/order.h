/**
 * @file
 * @brief Takes care of, execute and deletes orders.
 *
 *
 * Veglo, Lønvik
 */
#ifndef ORDERHANDLING_H
#define ORDERHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "door.h"

/**
 * @param dir Current movement.
 * @param save_dir Saves current movement, except HARDWARE_MOVEMENT_STOP.
 */

extern HardwareMovement dir;
extern HardwareMovement save_dir;

/**
 * @brief Takes care of orders by @param set_or_del integer at @param floor in @param order_type. @param set_or_del command order lights. 
 * 
 * @param floor Requested floor.
 * @param order_type 
 * @param set_or_del Add or remove bit from elevator que's.
 */
void elev_queue(int floor, HardwareOrder order_type, int set_or_del);

/**
 * @brief Updates the position of the elevator and checks if any buttons are pressed. 
 */
void check_buttons_update_floor();

/**
 * @brief Stops elevator if in requested floor, opens door and deletes order. 
 */
void stop_elev_open_door();

/**
 * @brief Checks if the elevator should continue the previous direction based on requested orders. 
 *
 * @return 1 if any orders in the same direction, 0 if not.
 */
int move_same_dir();

/**
 * @brief If stop button has been pressed, start the elevator when a new request occur. Will change direction if no orders in same direction.
 */
void from_stop_to_run();

/**
 * @brief Checks if the elevator should change the previous direction based on requested orders. 
 *
 * @return 1 if any orders in the opposite direction, 0 if not.
 */
int change_dir();


#endif