/*
 * DriveModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */
#include "DriveModule.h"

DriveModule::DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB) {
	rTalon1 = new CANTalon(rTal1);
	rTalon2 = new CANTalon(rTal2);
	lTalon1 = new CANTalon(lTal1);
	lTalon2 = new CANTalon(lTal2);

	rEnc = new Encoder(rEncA, rEncB);
	lEnc = new Encoder(lEncA, lEncB);

	driveIn = new DriveIn(rEnc, lEnc);
	driveOut = new DriveOut();
	drive_controller = new PIDController(DRIVE_CONTROLLER_P, DRIVE_CONTROLLER_I, DRIVE_CONTROLLER_D, driveIn, driveOut);
}

void DriveModule::setRightPower(double rPow) {
	rTalon1->Set(rPow);
	rTalon2->Set(rPow);
}

void DriveModule::setLeftPower(double lPow) {
	lTalon1->Set(lPow);
	lTalon2->Set(lPow);
}

void DriveModule::driveArcade(double throttle, double angle) {
	leftMotorOutput = 0;
	rightMotorOutput = 0;

	if(throttle > 0.0) {
		angle = -angle;
		if(angle < 0.0) {
			leftMotorOutput = (throttle + angle);
			rightMotorOutput = fmax(throttle, -angle);
		}
		else {
			leftMotorOutput = fmax(throttle, angle);
			rightMotorOutput = (throttle - angle);
		}
	}
	else {
		if(angle > 0.0) {
			leftMotorOutput = -fmax(-throttle, angle);
			rightMotorOutput = throttle + angle;
			//std::cout << rightMotorOutput << std::endl;
		}
		else {
			leftMotorOutput = throttle - angle;
			rightMotorOutput = -fmax(-throttle,-angle);
		}
	}
	setLeftPower(leftMotorOutput);
	setRightPower(rightMotorOutput);
}

void DriveModule::driveTank(double lPow, double rPow) {
	setLeftPower(lPow);
	setRightPower(rPow);
}

void DriveModule::setSetpoint(double setpoint) {
	drive_controller->SetSetpoint(setpoint);
}
