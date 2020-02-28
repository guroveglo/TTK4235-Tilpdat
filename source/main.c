#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "logic.h"
#include "order.h"
#include "door.h"


static void sigint_handler(int sig){
    (void)(sig);
    printf("\nTerminating 'Elevator to Heaven' --> Going to HELL!\n\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    
    signal(SIGINT, sigint_handler);

    printf("\n=== 'Elevator to Heaven', by Lønvik and Veglo ===\n");
   
    start_condition();

    while(1){
	    hardware_command_movement(dir);
        check_buttons_update_floor();
        
        if (move_same_dir()){
            stop_elev_open_door();
        }
        else if(change_dir()){
            stop_elev_open_door();
        }
        else if (dir == HARDWARE_MOVEMENT_STOP){
            from_stop_to_run();
        } 
    }
} 
