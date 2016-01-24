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
	IntakeModule(int spinInput, int moveInput);
	void setSpinPower(double pow);
	void setMovePower(double pow);
	void spinIn();
	void spinOut();

private:
	CANTalon* spinTalon;
	CANTalon* moveTalon;
};


#endif /* SRC_MODULES_INTAKEMODULE_H_ */
