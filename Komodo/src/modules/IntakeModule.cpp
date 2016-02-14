/*
 * IntakeModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#include <modules/IntakeModule.h>

IntakeModule::IntakeModule(int spinInput, int moveInput) : RobotModule("IntakeModule") {
	spinTalon = new CANTalon(spinInput);
	moveSol = new Solenoid(moveInput);

}

void IntakeModule::setSpinPower(double pow) {
	spinTalon->Set(pow);
}

void IntakeModule::retractIntake() {
	moveSol->Set(false);
}

void IntakeModule::deployIntake() {
	moveSol->Set(true);
}

void IntakeModule::alternateIntake() {
	moveSol->Set(!moveSol->Get());
}