/*
 * ShooterModule.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */
#include "WPILib.h"
#include "Settings.h"
#include <iostream>
#include <cstdio>
#include <ctime>

#ifndef SRC_MODULES_SHOOTERMODULE_H_
#define SRC_MODULES_SHOOTERMODULE_H_


class ShooterIn: public PIDSource{
private:
	AnalogPotentiometer* p1;

public:
	virtual ~ShooterIn(){};
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


	void PIDWrite(float output){
		power = output;
	}

	double getPower(){
		return power;
	}


};

class ShooterModule {
public:
	ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport, int solenoidPort);
	virtual ~ShooterModule();

	void setAngleMotorPower(double power);
	void tilt(double angle);
	void shoot(double left, double right, double time);

    void run();
    static void callrun(void*);
  	void createThread();

private:
	AnalogPotentiometer* angle;
	CANTalon* angleMotor;
	CANTalon* rightShooter;
	CANTalon* leftShooter;
	Solenoid* shooterSol;

	ShooterIn* shootIn;
	ShooterOut* shootOut;
	PIDController* angleController;
};

#endif /* SRC_MODULES_SHOOTERMODULE_H_ */
