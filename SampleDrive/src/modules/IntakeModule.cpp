/*
 * IntakeModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#include <modules/IntakeModule.h>

IntakeModule::IntakeModule(int spinInput, int spinInput2, int moveInput) {
	spinTalon = new CANTalon(spinInput);
	spinTalon2 = new CANTalon(spinInput2);
	moveSol = new Solenoid(moveInput);

}

void IntakeModule::setSpinPower(double pow) {
	spinTalon->Set(pow);
	spinTalon2->Set(pow);
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

bool IntakeModule::getStatus() {
	return moveSol->Get();
}
