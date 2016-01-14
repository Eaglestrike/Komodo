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
	IntakeModule(int intake);
	void setPower(double pow);
	void spinIn();
	void spinOut();

private:
	CANTalon* intakeTalon;
};


#endif /* SRC_MODULES_INTAKEMODULE_H_ */
