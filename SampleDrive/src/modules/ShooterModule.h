/*
 * ShooterModule.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */
#include "WPILib.h"

#ifndef SRC_MODULES_SHOOTERMODULE_H_
#define SRC_MODULES_SHOOTERMODULE_H_

class ShooterModule {
public:
	ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport);
	virtual ~ShooterModule();

	void tilt(double angle);
	void shoot();


private:
	AnalogPotentiometer* angle;
	CANTalon* angleMotor;
	CANTalon* right_shooter;
	CANTalon* left_shooter;
};

#endif /* SRC_MODULES_SHOOTERMODULE_H_ */
