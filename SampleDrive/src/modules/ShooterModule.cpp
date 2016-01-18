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
	left_shooter = new CANTalon(leftport);
	right_shooter = new CANTalon(rightport);

	angleMotor->SetFeedbackDevice(CANTalon::FeedbackDevice(2));

	//angleMotor->SetFeedbackDevice(angle);
	angleMotor->SetPID(0,0,0);

}

ShooterModule::~ShooterModule() {
	// TODO Auto-generated destructor stub
}


void ShooterModule::shoot(double left, double right, double time) {
	std::clock_t timer;
	timer = std::clock();
	left_shooter->Set(left);
	right_shooter->Set(right);
	if(timer >= time*CLOCKS_PER_SEC) {
		left_shooter->Set(0);
		right_shooter->Set(0);
	}
}

void ShooterModule::tilt(double angle) {
	angleMotor->SetSetpoint(angle);
}
