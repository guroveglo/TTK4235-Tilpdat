
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"


void floor_indicator(){
	int floor = read_floor();
	if (floor!=-1){
		hardware_command_floor_indicator_on(floor);
	}
}
