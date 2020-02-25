
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "door.h"
#include "order.h"


static int queue_inside[NUMB_FLOORS];
static int queue_up[NUMB_FLOORS];
static int queue_down[NUMB_FLOORS];


static int prev_floor;
static int current_floor;
static int above_floor;

HardwareMovement dir = HARDWARE_MOVEMENT_STOP;
HardwareMovement save_dir = HARDWARE_MOVEMENT_STOP;  


void elev_queue(int floor, HardwareOrder order_type, int set_or_del) {
    if (order_type == HARDWARE_ORDER_UP) {
        queue_up[floor] = set_or_del;
    }
    if (order_type == HARDWARE_ORDER_DOWN) {
        queue_down[floor] = set_or_del;
    }
    if (order_type == HARDWARE_ORDER_INSIDE) {
        queue_inside[floor] = set_or_del;
    }
   hardware_command_order_light(floor, order_type, set_or_del);
}

void check_buttons_update_floor(){
    int floor =read_floor();
    if (floor!=-1){
        prev_floor = floor;
        current_floor = prev_floor;
        above_floor = 0;
        if(dir==HARDWARE_MOVEMENT_DOWN){
            current_floor = prev_floor-1;
        }
    }
    else{
        above_floor =1;
    }
    floor_indicator();
	
    check_and_stop_elevator();
    for (int i = 0; i < NUMB_FLOORS; i++) {
            if (hardware_read_order(i, HARDWARE_ORDER_UP)) { 
                elev_queue(i, HARDWARE_ORDER_UP,1);                
            }
            if (hardware_read_order(i, HARDWARE_ORDER_DOWN)) { 
                elev_queue(i, HARDWARE_ORDER_DOWN,1);            
            }
            if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)) { 
                elev_queue(i, HARDWARE_ORDER_INSIDE,1);          
            }
        }
    }


void stop_elev_open_door(){

    for(int i=0; i< NUMB_FLOORS; i++){
        if ((queue_up[i] && read_floor()==i && dir == HARDWARE_MOVEMENT_UP) || 
            (queue_down[NUMB_FLOORS-1]&& read_floor()==NUMB_FLOORS-1 && i == NUMB_FLOORS-1) || 
            (queue_down[i] && read_floor()==i && dir == HARDWARE_MOVEMENT_DOWN) || 
            (queue_up[0]&& read_floor()==0 && i==0) ||
            (queue_inside[i]&&read_floor()==i))
        { 
			save_dir = dir;
            dir = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(dir);

            door();
            elev_queue(i,HARDWARE_ORDER_INSIDE,0);
            elev_queue(i,HARDWARE_ORDER_DOWN,0);
            elev_queue(i,HARDWARE_ORDER_UP,0);
        }
    }
}

int move_same_dir(){

    for(int j = prev_floor;j< NUMB_FLOORS;j++){   
	
        if(save_dir == HARDWARE_MOVEMENT_UP && (queue_up[j] || queue_inside[j] || queue_down[NUMB_FLOORS-1]) && !(stop_pressed)){
			dir = HARDWARE_MOVEMENT_UP;
			save_dir = dir;
            return 1;
        }

    }
    for(int j = prev_floor;j>=0;j--){    
        if(save_dir == HARDWARE_MOVEMENT_DOWN && (queue_down[j] || queue_inside[j] || queue_up[0]) && !(stop_pressed)){
			dir = HARDWARE_MOVEMENT_DOWN;
            save_dir = dir;
            return 1;
        }
    }

    return 0;
}

void from_stop_to_run(){
    int run = 0;
    int floor_req;
    for(int i=0;i<NUMB_FLOORS;i++){
        if(queue_up[i] || queue_inside[i] || queue_down[i]){
            run = 1;
            floor_req = i;
            break;
        }
    }
    if(run){
		if (floor_req == read_floor()) { 
			door();
            elev_queue(floor_req,HARDWARE_ORDER_INSIDE,0);
            elev_queue(floor_req,HARDWARE_ORDER_DOWN,0);
            elev_queue(floor_req,HARDWARE_ORDER_UP,0);
			
    	}
    	while (floor_req != read_floor()) {
            if ((current_floor==floor_req && above_floor)||current_floor>floor_req) {
                dir = HARDWARE_MOVEMENT_DOWN;  
                 for(int j = floor_req;j>=0;j--){        
                    if(queue_down[j] || queue_inside[j] || (j==0&&queue_up[0])){
                        floor_req=j;
                    }
                }
            }         
            
    		else if (current_floor<floor_req) {
    			dir = HARDWARE_MOVEMENT_UP;
                for(int j = floor_req;j< NUMB_FLOORS;j++){   
                    if(queue_up[j] || queue_inside[j] || (j==NUMB_FLOORS-1 && queue_down[NUMB_FLOORS-1])){
                        floor_req = j;
                    } 
                }
    			
    		}

            if (floor_req==read_floor()){
                 save_dir = dir;
            }

            hardware_command_movement(dir);
            check_buttons_update_floor();  

            if (dir==HARDWARE_MOVEMENT_STOP){
                break;
            }
            stop_elev_open_door();
    	}
    }

    if(dir!=HARDWARE_MOVEMENT_STOP){
        stop_pressed=0;
    }
} 

int change_dir(){
    for(int j = prev_floor;j>=0;j--){
        if(save_dir == HARDWARE_MOVEMENT_UP && (queue_up[j]||queue_down[j]||queue_inside[j])&& !(stop_pressed)){
            dir = HARDWARE_MOVEMENT_DOWN;
            save_dir = dir;
            return 1;
        }
    }
    for(int j = prev_floor;j< NUMB_FLOORS;j++){
         if(save_dir == HARDWARE_MOVEMENT_DOWN && (queue_up[j]||queue_down[j]||queue_inside[j])&& !(stop_pressed)){
            dir = HARDWARE_MOVEMENT_UP;
            save_dir=dir;
            return 1;
        }
    }
    return 0;
}


