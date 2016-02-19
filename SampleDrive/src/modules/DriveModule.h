/*
 * DriveModule.h
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#ifndef SRC_MODULES_DRIVEMODULE_H_
#define SRC_MODULES_DRIVEMODULE_H_

#include <WPILib.h>
#include "Settings.h"

class DriveIn: public PIDSource {
private:
	Encoder* rEnc;
	Encoder* lEnc;

public:
	virtual ~DriveIn(){}
	DriveIn(Encoder* rEncInput, Encoder* lEncInput) {
		rEnc = rEncInput;
		lEnc = lEncInput;
	}

	double PIDGet() {
		return (rEnc->PIDGet() + lEnc->PIDGet())/2.0;
	}
};

class AngleIn: public PIDSource {
private:
	Encoder* rEnc;
	Encoder* lEnc;

public:
	virtual ~AngleIn(){}
	AngleIn(Encoder* rEncInput, Encoder* lEncInput) {
		rEnc = rEncInput;
		lEnc = lEncInput;
	}

	double PIDGet() {
		return (rEnc->GetDistance() - lEnc->GetDistance());
	}
};

class DriveOut: public PIDOutput {
private:
	double power;

public:
	DriveOut(): power(0) {}
	virtual ~DriveOut(){}

	void PIDWrite(float output) {
		power = output;
	}

	double getPower() {
		return power;
	}
};

class DriveModule{
public:
	DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB);
	void setRightPower(double rPow);
	void setLeftPower(double lPow);
	void driveArcade(double throttle, double angle);
	void driveTank(double lPow, double rPow);
	void setDriveSetpoint(double setpoint);
	double getDriveSetpoint();
	void setDrivePID(double p, double i, double d);
	double getDriveOutput();
	double getRightEncoder();
	double getLeftEncoder();
	void setPID(double p, double i, double d);
	double getD();
	double getI();
	double getP();
	void EnablePID(bool enable);
	double getAngleOutput();
	void setAngleSetpoint(double angle);
	double getAngleSetpoint();
	void drive(double setpoint);
	void turn(double angle);
private:
	CANTalon* rTalon1;
	CANTalon* rTalon2;
	CANTalon* lTalon1;
	CANTalon* lTalon2;
	double leftMotorOutput;
	double rightMotorOutput;

	Encoder* lEnc;
	Encoder* rEnc;
	DriveIn* driveIn;
	DriveOut* driveOut;
	AngleIn* angleIn;
	DriveOut* angleOut;
	PIDController* drive_controller;
	PIDController* angle_controller;
};



#endif /* SRC_MODULES_DRIVEMODULE_H_ */
