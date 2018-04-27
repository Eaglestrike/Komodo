/*
 * ShooterModule.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */
#include "WPILib.h"
#include <ctre/Phoenix.h>
#include "Settings.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include "RobotModule.h"

#ifndef SRC_MODULES_SHOOTERMODULE_H_
#define SRC_MODULES_SHOOTERMODULE_H_


class ShooterIn: public PIDSource {
private:
	AnalogPotentiometer* p1;

public:
	virtual ~ShooterIn() {};
	ShooterIn(AnalogPotentiometer* a){
		p1 = a;
	}

	double PIDGet(){
		return p1->Get();
	}

};

class ShooterOut: public PIDOutput{
private:
	double power;

public:
	virtual ~ShooterOut(){};
	ShooterOut(): power(0){}


	void PIDWrite(double output){
		power = output;
	}

	double getPower(){
		return power;
	}


};

class ShooterModule: public RobotModule {
public:
	ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport, int solenoidPort, int buttonport);
	virtual ~ShooterModule();

	void setAngleMotorPower(double power);
	void tilt(double angle);
	void shoot(double left, double right, double time);
	bool getShot();
	double getAngle();
	void mShoot(double power);
	void shootKicker(bool kick);
	bool isBallIn();
	double getP();
	double getI();
	double getD();

	void setShooterSpeed(double speed);

	void setPID(double p, double i, double d);
    void run();
    void enablePID();
    void setMaxPower(double power);
    double getSetpoint();
    static void callrun(void*);
  	void createThread();
  	void speed(double);
  	double getSpeed();
  	void disablePID();
private:
	AnalogPotentiometer* angle;
	WPI_TalonSRX* angleMotor;
	WPI_TalonSRX* rightShooter;
	WPI_TalonSRX* leftShooter;
	Solenoid* shooterSol;

	ShooterIn* shootIn;
	ShooterOut* shootOut;
	PIDController* angleController;
	DigitalInput* button;
	bool shot = false;
};

#endif /* SRC_MODULES_SHOOTERMODULE_H_ */