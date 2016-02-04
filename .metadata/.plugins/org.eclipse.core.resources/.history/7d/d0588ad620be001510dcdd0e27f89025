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
	angleMotor = new CanTalonSRX(angleMotorPort);
	left_shooter = new CanTalonSRX(leftport);
}

ShooterModule::~ShooterModule() {
	// TODO Auto-generated destructor stub
}

