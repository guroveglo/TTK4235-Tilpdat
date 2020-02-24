
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"



void start_condition() {
    reset_lights();
    while (read_floor()!=0) {
       hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
       floor_indicator();
    }
    dir = HARDWARE_MOVEMENT_STOP;
    hardware_command_movement(dir);
    
    prev_floor=0;
    return;
}

int read_floor() {
    for (int i = 0; i < NUMB_FLOORS; i++) {
        if (hardware_read_floor_sensor(i)) {
            return i;
        }
    }
    return -1;
}

void reset_lights(){
    for (int i = 0; i < NUMB_FLOORS; i++){  
            elev_que(i, HARDWARE_ORDER_UP,0);
            elev_que(i, HARDWARE_ORDER_DOWN,0);
            elev_que(i, HARDWARE_ORDER_INSIDE,0);
        } 
}

void check_and_stop_elevator(){
	
    if(hardware_read_stop_signal()){
        reset_lights();   
        dir = HARDWARE_MOVEMENT_STOP;   
        hardware_command_movement(dir);
        while(hardware_read_stop_signal()){
            hardware_command_stop_light(1);
            stopbutton_door_open();
        }        
        hardware_command_stop_light(0);
        stop_pressed = 1;

        //printf("stop: %d ", stop_pressed);
        /*printf("prev_floor: %d ", prev_floor);
        printf("Current_floor: %d ", current_floor);
        printf("Above: %d ", above_floor);*/

    }

}

void timer(){
	time_t start_time = time(NULL);
	time_t current_time = time(NULL);
    double time_used = difftime(current_time,start_time);
     
    while (time_used<3.0){
		check_buttons();

		if(hardware_read_obstruction_signal()||hardware_read_stop_signal()){
			start_time = time(NULL);
		
		}
		current_time = time(NULL);
		time_used = difftime(current_time,start_time);
		
		if (!hardware_read_stop_signal()){
			hardware_command_stop_light(0);
		}

	}
}
