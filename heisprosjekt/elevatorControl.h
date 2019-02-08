//
//  elevatorControl.h
//  Heisprosjekt2.0
//
//  Created by Vebjørn Steinsholt on 08.03.2017.
//  Copyright © 2017 Vebjørn Steinsholt. All rights reserved.
//

#ifndef elevatorControl_h
#define elevatorControl_h

#include <stdio.h>
#include "elev.h"
#include <stdbool.h>

#endif /* elevatorControl_h */
void addToQueue(elev_button_type_t button, int floor);
void removeFromQueue(int floor);
void startUp();
bool stopOnFloor(int floor, int currentFloor);
void getOrders();
bool isOrdersBelow(int currentFloor);
bool isOrdersAbove(int currentFloor);
void chooseDirection(int currentFloor);
void getCurrentFloor();
void serviceFloor();
void resetQueue();
void floorIndicator();
