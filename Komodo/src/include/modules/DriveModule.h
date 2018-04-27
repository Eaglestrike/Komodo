/*
 * DriveModule.h
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */

#ifndef SRC_MODULES_DRIVEMODULE_H_
#define SRC_MODULES_DRIVEMODULE_H_

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include "AHRS.h"
#include "Settings.h"
#include "CameraInput.h"
#include "RobotModule.h"

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
		return rEnc->PIDGet();
	}
};

class AngleIn: public PIDSource {
private:
	AHRS* gyro;

public:
	virtual ~AngleIn(){}
	AngleIn(AHRS* g) {
		gyro = g;
	}

	double PIDGet() {
		return gyro->GetYaw();
	}
};

class DriveOut : public PIDOutput  {

public:
	DriveOut() {}
	virtual ~DriveOut(){}

	void PIDWrite(double output) {
		power = output;
	}

	double getPower() {
		return power;
	}

private:
	double power = 0;
};

class DriveModule: public RobotModule {
public:
	DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB, PIDSource* panIn, AHRS* gyro);
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
	void driveWithoutAngle(double setpoint);
	void turn(double angle);
	void setPanPID(double p, double i, double d);
	double getPanP();
	double getPanI();
	double getPanD();
	void setPanSetpoint(double setPoint);
	void enablePan(bool enable);
	void turnALPHA(double angle);
	void calibrate();
	double getPanOutput();
	double getPanSetpoint();
	double getPanInput();
	double getAngle();
	void setMaxPower(double min, double max);
	void reset();
	void resetEncoders();

private:
	WPI_TalonSRX* rTalon1;
	WPI_TalonSRX* rTalon2;
	WPI_TalonSRX* lTalon1;
	WPI_TalonSRX* lTalon2;
	double leftMotorOutput;
	double rightMotorOutput;

	Encoder* lEnc;
	Encoder* rEnc;
	DriveIn* driveIn;
	DriveOut* driveOut;
	AngleIn* angleIn;
	DriveOut* angleOut;
	DriveOut* panOut;
	PIDController* drive_controller;
	PIDController* angle_controller;
	PIDController* pan_controller;
	PIDSource* pan;
	AHRS* gyro;
};



#endif /* SRC_MODULES_DRIVEMODULE_H_ */