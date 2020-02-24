/**
 * @file
 * @brief Door handling. 
 *
 *
 * Veglo, Lønvik
 */
#ifndef DOOR
#define DOOR
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"


/**
 * @brief Opens door, sets doorlight.
 */
void door();
/**
 * @brief If stopbutton is pressed on floor, call door. 
 */
void stopbutton_door_open();

#endif