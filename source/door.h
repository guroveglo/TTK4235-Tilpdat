/**
 * @file
 * @brief Door controller.
 *
 *
 * Veglo, Lønvik
 */
#ifndef DOOR_H
#define DOOR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hardware.h"
#include "logic.h"
#include "door.h"


/**
 * @brief Opens door and turns on/off door light.
 */
void door();
/**
 * @brief If the stop button is pressed while on a floor, call door(). 
 */
void stopbutton_door_open();

#endif