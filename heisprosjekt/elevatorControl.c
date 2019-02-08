//
//  elevatorControl.c
//  Heisprosjekt2.0
//
//  Created by Vebjørn Steinsholt on 08.03.2017.
//  Copyright © 2017 Vebjørn Steinsholt. All rights reserved.
//
#include "elevatorControl.h"
static int currentFloor=-1;
static elev_motor_direction_t currentDir=DIRN_STOP;
static int queue[N_FLOORS][N_BUTTONS] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};
void addToQueue(elev_button_type_t button, int floor){
    queue[floor][button]=1;
    elev_set_button_lamp(button, floor, 1);
}
void resetQueue(){
    for (int i=0; i<N_FLOORS; i++) {
        removeFromQueue(i);
    }
}
void removeFromQueue(int floor){
    if (floor!=0) {
        elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
        queue[floor][BUTTON_CALL_DOWN]=0;
    }
    if (floor!=3) {
        elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
        queue[floor][BUTTON_CALL_UP]=0;
    }
    queue[floor][BUTTON_COMMAND]=0;
    elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
}
void getCurrentFloor(){
    int placeHolderFloor=0;
    while (elev_get_floor_sensor_signal()==-1) {
        placeHolderFloor=elev_get_floor_sensor_signal();
    }
    currentFloor=placeHolderFloor;
}
void floorIndicator(){
    if(currentFloor!=-1){
    elev_set_floor_indicator(currentFloor);
}
}
void startUp(){
    while (elev_get_floor_sensor_signal()==-1) {
        elev_set_motor_direction(DIRN_DOWN);
        currentDir=DIRN_DOWN;
    }
    if (elev_get_floor_sensor_signal()!=-1) {
        elev_set_motor_direction(DIRN_STOP);
    }
}
bool stopOnFloor(int floor, int currentFloor){
    if (currentDir==DIRN_UP) {
        if (queue[floor][BUTTON_CALL_UP]==1) {
                  return true;
        }
    }
        if (currentDir==DIRN_DOWN) {
            if (queue[floor][BUTTON_CALL_DOWN]==1) {
                return true;
            }
        }
    if (currentDir==DIRN_UP && isOrdersAbove(currentFloor)) {
        return(queue[floor][BUTTON_COMMAND]==1);
        
        }
    if (currentDir==DIRN_DOWN && isOrdersBelow(currentFloor)) {
        return queue[floor][BUTTON_COMMAND]==1;
    }
    return false;
}

bool isOrdersBelow(int currentFloor){
    if (currentFloor==0) {
        return false;
    }
    for (int i=0; i<currentFloor; i++) {
        if (queue[i][BUTTON_CALL_UP]==1) {
            return true;
        }
        if (queue[i][BUTTON_CALL_DOWN]==1) {
            return true;
        }
        if (queue[i][BUTTON_COMMAND]==1) {
            return true;
        }
    }
    return false;
}
bool isOrdersAbove(int currentFloor){
    if (currentFloor==3) {
        return false;
    }
    for (int i=currentFloor+1; i<N_FLOORS; i++) {
        if (queue[i][BUTTON_CALL_UP]==1) {
            return true;
        }
        if (queue[i][BUTTON_CALL_DOWN]==1) {
            return true;
        }
        if (queue[i][BUTTON_COMMAND]==1) {
            return true;
        }
    }
    return false;
    
}
void getOrders(){
    for (int i=0; i<N_FLOORS; i++) {
        if (elev_get_button_signal(BUTTON_COMMAND, i)) {
            addToQueue(BUTTON_COMMAND, i);
        }
        if (elev_get_button_signal(BUTTON_CALL_DOWN, i)) {
            addToQueue(BUTTON_CALL_DOWN, i);
        }
        if (elev_get_button_signal(BUTTON_CALL_UP, i)) {
            addToQueue(BUTTON_CALL_UP, i);
        }
    }
}
void chooseDirection(int currentFloor){
    if (currentDir==DIRN_UP) {
        if (isOrdersAbove(currentFloor)) {
            return;
        }else if (isOrdersBelow(currentFloor))
            currentDir=DIRN_DOWN;
        return;
    }
    if (currentDir==DIRN_DOWN) {
        if (isOrdersBelow(currentFloor)) {
            return;
        }else if (isOrdersAbove(currentFloor)){
            currentDir=DIRN_UP;
        }
    }
}
