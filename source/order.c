
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"
#include "order.h"


static int queue_inside[NUMB_FLOORS];
static int queue_up[NUMB_FLOORS];
static int queue_down[NUMB_FLOORS];



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

void check_buttons(){
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
            if (hardware_read_order(i, HARDWARE_ORDER_UP)) { //sjekker om oppknapp er trykket
                elev_queue(i, HARDWARE_ORDER_UP,1);                //legger i que
            }
            if (hardware_read_order(i, HARDWARE_ORDER_DOWN)) { //sjekker om nedknapp er trykket
                elev_queue(i, HARDWARE_ORDER_DOWN,1);            // sender til que
            }
            if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)) { //sjekker om innsideknapp er trykket
                elev_queue(i, HARDWARE_ORDER_INSIDE,1);            // sender til que
            }
        }
    }


void stop_elev_open_door(){

    for(int i=0; i< NUMB_FLOORS; i++){
        if ((queue_up[i]==1 && read_floor()==i && dir == HARDWARE_MOVEMENT_UP)|| (queue_down[NUMB_FLOORS-1]==1 && read_floor()==NUMB_FLOORS-1 && i == NUMB_FLOORS-1) || (queue_inside[i]==1&&read_floor()==i) || (queue_down[i]==1 && read_floor()==i && dir == HARDWARE_MOVEMENT_DOWN) || (queue_up[0]==1 && read_floor()==0 && i==0)){  //stopp på etasje
			save_dir = dir;
            dir = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(dir);

            door();
            if(queue_inside[i]==1){
                elev_queue(i,HARDWARE_ORDER_INSIDE,0);
            }
            if((queue_up[i]==1&&save_dir == HARDWARE_MOVEMENT_UP) || i==NUMB_FLOORS-1){
                elev_queue(i,HARDWARE_ORDER_UP,0);
                elev_queue(i,HARDWARE_ORDER_DOWN,0);
            }
            if((queue_down[i]==1&&save_dir==HARDWARE_MOVEMENT_DOWN) || i==0){
                elev_queue(i,HARDWARE_ORDER_DOWN,0);
                elev_queue(i,HARDWARE_ORDER_UP,0);
            }
        }
    }
}

int move_same_dir(){

    for(int j = prev_floor;j< NUMB_FLOORS;j++){   //Fortsett opp
	
        if(save_dir == HARDWARE_MOVEMENT_UP && (queue_up[j]==1 || queue_inside[j]==1 || queue_down[NUMB_FLOORS-1]==1) && !(stop_pressed)){
			dir = HARDWARE_MOVEMENT_UP;
			save_dir = dir;
            return 1;
        }

    }
    for(int j = prev_floor;j>=0;j--){        //fortsett ned
        if(save_dir == HARDWARE_MOVEMENT_DOWN && (queue_down[j]==1 || queue_inside[j]==1 || queue_up[0]==1) && !(stop_pressed)){
			dir = HARDWARE_MOVEMENT_DOWN;
            save_dir = dir;
            return 1;
        }
    }

    return -1;
}

void from_stop_to_run(){
    int run = 0;
    int floor_req;
    for(int i=0;i<NUMB_FLOORS;i++){
        if(queue_up[i]==1 || queue_inside[i]==1 || queue_down[i]==1){
            run = 1;
            floor_req = i;
            break;
        }
    }
    if(run){
		if (floor_req == read_floor()) { //hvis i etasje
			door();
			queue_inside[floor_req] = 0;
			queue_up[floor_req] = 0;
			queue_down[floor_req] = 0;
			
    	}
    	while (floor_req != read_floor()) {

            if ((current_floor==floor_req && above_floor)||floor_req<current_floor) {
                dir = HARDWARE_MOVEMENT_DOWN;  
                 for(int j = prev_floor;j>=0;j--){        //fortsett ned
                    if(queue_down[j]==1 || queue_inside[j]==1 || queue_up[0]==1){
                        floor_req=j;
                    }
                }
            }      
                
            
    		else if (current_floor<floor_req) {
    			dir = HARDWARE_MOVEMENT_UP;
                for(int j = prev_floor;j< NUMB_FLOORS;j++){   //Fortsett opp
                    if(queue_up[j]==1 || queue_inside[j]==1 || queue_down[NUMB_FLOORS-1]==1){
                        floor_req = j;
                    } 
                }
    			
    		}


            if (floor_req==read_floor()){
                 save_dir = dir;
            }

            hardware_command_movement(dir);
            check_buttons();  //må ha check buttons her for å kunne stoppe mellom etasjene
            stop_elev_open_door();
            if (dir==HARDWARE_MOVEMENT_STOP){
                break;
            }
    	}
    }

    if(dir!=HARDWARE_MOVEMENT_STOP){
        stop_pressed=0;
    }
} 

int change_dir(){
    //hvis nedover og oppoverknapp trykket
    for(int j = prev_floor;j>=0;j--){
        if(save_dir == HARDWARE_MOVEMENT_DOWN && (queue_up[j]||queue_down[j]||queue_inside[j])&& !(stop_pressed)){
            dir = HARDWARE_MOVEMENT_UP;
            save_dir = dir;
            return 3;
        }
    }
    for(int j = prev_floor;j< NUMB_FLOORS;j++){
         if(save_dir == HARDWARE_MOVEMENT_UP && (queue_up[j]||queue_down[j]||queue_inside[j])&& !(stop_pressed)){
            dir = HARDWARE_MOVEMENT_DOWN;
            save_dir=dir;
            return 3;
        }
    }
    return -1;

    //hvis oppover og nedoverknapp trykket
}

void order_handling() {
    //start_condition gir STOP
	//start_condition gir etg 0

    hardware_command_movement(dir);

    check_buttons();
    
    
    
    int case_int;
    if (move_same_dir() == 1){
        case_int = 1;
    }
    else if(change_dir()==3){
        printf("vi er her");
        case_int =3;
    }
    else if (dir == HARDWARE_MOVEMENT_STOP){
        case_int=2;
    }

     

    switch(case_int){
        case 0:
            break;

        case 1:
            stop_elev_open_door();
            break;

           
        case 2:
            from_stop_to_run();
            break;

        case 3:
            stop_elev_open_door();
            break;

        default:
            break;
    }
}

