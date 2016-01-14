/*
 * DriveModule.h
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#ifndef SRC_MODULES_DRIVEMODULE_H_
#define SRC_MODULES_DRIVEMODULE_H_

#include <WPILib.h>

class DriveModule{
public:
	DriveModule(int lTal1, int lTal2, int rTal1, int rTal2);
	void setRightPower(double rPow);
	void setLeftPower(double lPow);
	void driveArcade(double throttle, double angle);
	void driveTank(double lPow, double rPow);

private:
	CANTalon* rTalon1;
	CANTalon* rTalon2;
	CANTalon* lTalon1;
	CANTalon* lTalon2;
	double leftMotorOutput;
	double rightMotorOutput;
};



#endif /* SRC_MODULES_DRIVEMODULE_H_ */
