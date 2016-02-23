/*
 * IntakeModule.h
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#ifndef SRC_MODULES_INTAKEMODULE_H_
#define SRC_MODULES_INTAKEMODULE_H_

#include <WPIlib.h>

class IntakeModule {
public:
	IntakeModule(int spinInput, int spinInput2, int moveInput);
	void setSpinPower(double pow);
	void spinIn();
	void spinOut();
	void deployIntake();
	void retractIntake();
	void alternateIntake();
	bool getStatus();

private:
	CANTalon* spinTalon;
	CANTalon* spinTalon2;
	Solenoid* moveSol;
};


#endif /* SRC_MODULES_INTAKEMODULE_H_ */
