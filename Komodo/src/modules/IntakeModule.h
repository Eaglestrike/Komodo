/*
 * IntakeModule.h
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#ifndef SRC_MODULES_INTAKEMODULE_H_
#define SRC_MODULES_INTAKEMODULE_H_

#include <WPIlib.h>
#include "RobotModule.h"

class IntakeModule: public RobotModule{
public:
	IntakeModule(int spinInput, int moveInput);
	void setSpinPower(double pow);
	void spinIn();
	void spinOut();
	void deployIntake();
	void retractIntake();
	void alternateIntake();

private:
	CANTalon* spinTalon;
	Solenoid* moveSol;
};


#endif /* SRC_MODULES_INTAKEMODULE_H_ */
