/*
 * ShooterModule.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */

#include <modules/ShooterModule.h>



ShooterModule::ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport, int solenoidPort) {
	// TODO Auto-generated constructor stub
	angle = new AnalogPotentiometer(anglePort);
	angleMotor = new CANTalon(angleMotorPort);
	leftShooter = new CANTalon(leftport);
	rightShooter = new CANTalon(rightport);
	shooterSol = new Solenoid(solenoidPort);

	shootIn = new ShooterIn(angle);
	shootOut = new ShooterOut();
	angleController = new PIDController(RSHOOTER_CONTROLLER_P, RSHOOTER_CONTROLLER_I, RSHOOTER_CONTROLLER_D,  shootIn, angleMotor);

}

ShooterModule::~ShooterModule() {
	// TODO Auto-generated destructor stub
}

void ShooterModule::createThread() {
	std::thread t (ShooterModule::callrun, this);
	t.detach();
}

void ShooterModule::callrun(void* m) {
	((ShooterModule*)m)->run();
}

void ShooterModule::run() {
	shoot(1, 1, 1);
}

bool ShooterModule::getShot() {
	return shot;
}
void ShooterModule::shoot(double left, double right, double time) {
	shot = true;
	shooterSol->Set(true);
	leftShooter->Set(-left);
	rightShooter->Set(right);
	Wait(time);
	leftShooter->Set(0);
	rightShooter->Set(0);
	shooterSol->Set(false);
	shot = false;
}

void ShooterModule::shootKicker(bool kick) {
	shooterSol->Set(kick);
}

void ShooterModule::mShoot(double power) {
	rightShooter->Set(power);
	leftShooter->Set(-power);

}
void ShooterModule::setAngleMotorPower(double power) {
	angleMotor->Set(power);
}

void ShooterModule::tilt(double angle) {
	angleController->SetSetpoint(angle);
}
