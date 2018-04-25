/*
 * Xbox.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: LAHS
 */

#include <Xbox.h>

Xbox::Xbox(int port): Joystick(port){

}


bool Xbox::getA(){
	return GetRawButton(1);
}

bool Xbox::getB(){
	return GetRawButton(2);
}

bool Xbox::getX(){
	return GetRawButton(3);
}

bool Xbox::getY(){
	return GetRawButton(4);
}

bool Xbox::getRB(){
	return GetRawButton(6);
}

bool Xbox::getLB(){
	return GetRawButton(5);
}

bool Xbox::getStart(){
	return GetRawButton(8);
}

bool Xbox::getBack(){
	return GetRawButton(7);
}

bool Xbox::getR3(){
	return GetRawButton(10);
}

bool Xbox::getL3(){
	return GetRawButton(9);
}



float Xbox::getLT(){
	return GetRawAxis(3);
}

float Xbox::getRT(){
	return GetRawAxis(3);
}

float Xbox::getRX(){
	return GetRawAxis(4);
}

float Xbox::getRY(){
	return -GetRawAxis(5);
}

float Xbox::getLX(){
	return GetX();
}


bool Xbox::getButtonPress(button button){
	if(GetRawButton(button)&&!buttonStatus[button]){
		buttonStatus[button]=true;
		return 1;
	}
	else{
		buttonStatus[button]=GetRawButton(button);
		return 0;
	}

}


float Xbox::getLY(){
	return -GetY();
}
void Xbox::setRumble(float rumble){
	SetRumble (kLeftRumble, rumble);
	SetRumble (kRightRumble, rumble);
}
void Xbox::setLRumble(float rumble){
	SetRumble (kLeftRumble, rumble);
}
void Xbox::setRRumble(float rumble){
	SetRumble (kRightRumble, rumble);
}
