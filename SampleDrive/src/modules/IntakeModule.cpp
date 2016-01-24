/*
 * IntakeModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#include <modules/IntakeModule.h>

IntakeModule::IntakeModule(int spinInput, int moveInput) {
	spinTalon = new CANTalon(spinInput);
	moveTalon = new CANTalon(moveInput);

}

void IntakeModule::setSpinPower(double pow) {
	spinTalon->Set(pow);
}

void IntakeModule::setMovePower(double pow) {
	moveTalon->Set(pow);
}
