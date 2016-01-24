/*
 * ShooterModule.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */

#include <modules/ShooterModule.h>



ShooterModule::ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport) {
	// TODO Auto-generated constructor stub
	angle = new AnalogPotentiometer(anglePort);
	angleMotor = new CANTalon(angleMotorPort);
	leftShooter = new CANTalon(leftport);
	rightShooter = new CANTalon(rightport);

	shootIn = new ShooterIn(angle);
	shootOut = new ShooterOut();
	angleController = new PIDController(SHOOTER_CONTROLLER_P, SHOOTER_CONTROLLER_I, SHOOTER_CONTROLLER_D,  shootIn, angleMotor);

}

ShooterModule::~ShooterModule() {
	// TODO Auto-generated destructor stub
}


void ShooterModule::shoot(double left, double right, double time) {
	Timer timer;
	timer.Start();
	leftShooter->Set(left);
	rightShooter->Set(right);
	if(timer.Get() >= time) {
		leftShooter->Set(0);
		rightShooter->Set(0);
	}
}

void ShooterModule::setAngleMotorPower(double power) {
	angleMotor->Set(power);
}

void ShooterModule::tilt(double angle) {
	angleController->SetSetpoint(angle);
}
