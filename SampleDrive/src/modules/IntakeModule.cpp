/*
 * IntakeModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#include <modules/IntakeModule.h>

IntakeModule::IntakeModule(int intake) {
	intakeTalon = new CANTalon(intake);
}

void IntakeModule::setPower(double pow) {
	intakeTalon->Set(pow);
}
