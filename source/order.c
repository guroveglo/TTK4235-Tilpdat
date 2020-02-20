
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "logic.h"
#include "light.h"
#include "door.h"
#include "order.h"


static int que_inside[NUMB_FLOORS];
static int que_up[NUMB_FLOORS];
static int que_down[NUMB_FLOORS];



void elev_que(int floor, HardwareOrder order_type, int set_or_del) {
    if (order_type == HARDWARE_ORDER_UP) {
        que_up[floor] = set_or_del;
    }
    if (order_type == HARDWARE_ORDER_DOWN) {
        que_down[floor] = set_or_del;
    }
    if (order_type == HARDWARE_ORDER_INSIDE) {
        que_inside[floor] = set_or_del;
    }
   hardware_command_order_light(floor, order_type, set_or_del);
}

void check_buttons(){
    if (read_floor()!=-1){
        prev_floor = read_floor();
        current_floor = prev_floor;
        above_floor = 0;
    }
    else{
        above_floor =1;
    }
	check_and_stop_elevator();
	//floor_indicator();
    for (int i = 0; i < NUMB_FLOORS; i++) {
            if (hardware_read_order(i, HARDWARE_ORDER_UP)) { //sjekker om oppknapp er trykket
                elev_que(i, HARDWARE_ORDER_UP,1);                //legger i que
            }
            if (hardware_read_order(i, HARDWARE_ORDER_DOWN)) { //sjekker om nedknapp er trykket
                elev_que(i, HARDWARE_ORDER_DOWN,1);            // sender til que
            }
            if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)) { //sjekker om innsideknapp er trykket
                elev_que(i, HARDWARE_ORDER_INSIDE,1);            // sender til que
            }
        }
    }


void stop_elev_open_door(){

    for(int i=0; i< NUMB_FLOORS; i++){
        if ((que_up[i]==1 && read_floor()==i && dir == HARDWARE_MOVEMENT_UP)|| (que_down[NUMB_FLOORS-1]==1 && read_floor()==NUMB_FLOORS-1 && i == NUMB_FLOORS-1) || (que_inside[i]==1&&read_floor()==i) || (que_down[i]==1 && read_floor()==i && dir == HARDWARE_MOVEMENT_DOWN) || (que_up[0]==1 && read_floor()==0 && i==0)){  //stopp på etasje
			prev_dir = dir;
            dir = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(dir);
            door();
            if(que_inside[i]==1){
                elev_que(i,HARDWARE_ORDER_INSIDE,0);
            }
            if((que_up[i]==1&&prev_dir == HARDWARE_MOVEMENT_UP) || i==NUMB_FLOORS-1){
                elev_que(i,HARDWARE_ORDER_UP,0);
                elev_que(i,HARDWARE_ORDER_DOWN,0);
            }
            if((que_down[i]==1&&prev_dir==HARDWARE_MOVEMENT_DOWN) || i==0){
                elev_que(i,HARDWARE_ORDER_DOWN,0);
                elev_que(i,HARDWARE_ORDER_UP,0);
            }
        }
    }
}

int move_same_dir(){

    for(int j = prev_floor;j< NUMB_FLOORS;j++){   //Fortsett opp
	
        if(prev_dir == HARDWARE_MOVEMENT_UP && (que_up[j]==1 || que_inside[j]==1 || que_down[NUMB_FLOORS-1]==1) && !(stop_pressed)){
			dir = HARDWARE_MOVEMENT_UP;
			prev_dir = dir;
            return 1;
        }

    }
    for(int j = prev_floor;j>=0;j--){        //fortsett ned
        if(prev_dir == HARDWARE_MOVEMENT_DOWN && (que_down[j]==1 || que_inside[j]==1 || que_up[0]==1) && !(stop_pressed)){
			dir = HARDWARE_MOVEMENT_DOWN;
            current_floor = prev_floor -1;
			prev_dir = dir;
            return 1;
        }
    }

    return -1;
}

void from_stop_to_run(){

    int run = 0;
    int floor_req;
    for(int i=0;i<NUMB_FLOORS;i++){
        if(que_up[i]==1 || que_inside[i]==1 || que_down[i]==1){
            run = 1;
            floor_req = i;
            break;
        }
    }
    if(run){
       
		if (floor_req == read_floor()) { //hvis i etasje
			door();
			que_inside[floor_req] = 0;
			que_up[floor_req] = 0;
			que_down[floor_req] = 0;
			
		}
		while (floor_req != read_floor()) {
            if ((current_floor==floor_req && above_floor)||floor_req<current_floor) {
                //printf("ned");
                dir = HARDWARE_MOVEMENT_DOWN;      
                current_floor = prev_floor -1;       

                
            }
			else if (current_floor<floor_req) {
                //printf("opp");
				dir = HARDWARE_MOVEMENT_UP;
				
			}

            if (floor_req==read_floor()){
                 prev_dir = dir;
            }

            hardware_command_movement(dir);
            check_buttons();  //må ha check buttons her for å kunne stoppe mellom etasjene
            
            if (dir==HARDWARE_MOVEMENT_STOP){
                break;
            }
		}
        if(dir!=HARDWARE_MOVEMENT_STOP){
            stop_pressed=0;
           
        }

        
           
    }
} 

int change_dir(){
    //hvis nedover og oppoverknapp trykket
    for(int j = prev_floor;j>=0;j--){
        if(prev_dir == HARDWARE_MOVEMENT_DOWN && que_up[j]==1 && read_floor()==j && !(stop_pressed)){
            dir = HARDWARE_MOVEMENT_UP;
            prev_dir = dir;
            return 3;
        }
    }
    for(int j = prev_floor;j< NUMB_FLOORS;j++){
         if(prev_dir == HARDWARE_MOVEMENT_UP && que_down[j]==1 && read_floor()==j && !(stop_pressed)){
            dir = HARDWARE_MOVEMENT_DOWN;
            current_floor = prev_floor -1;
            prev_dir=dir;
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
    if (move_same_dir() == 1 && !(stop_pressed)){
        case_int = 1;
    }
    else if(change_dir()==3 && !(stop_pressed)){
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

