
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"




void door(){
	
	hardware_command_door_open(1);
	timer();
	hardware_command_door_open(0);
}


void stopbutton_door_open(){

	if(read_floor()!=-1){
		door();
	}

}