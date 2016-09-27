/*
 * DriveModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */
#include "DriveModule.h"

DriveModule::DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB, PIDSource*  panIn, AHRS* gyro) : RobotModule("DriveModule") {
	rTalon1 = new CANTalon(rTal1);
	rTalon2 = new CANTalon(rTal2);
	lTalon1 = new CANTalon(lTal1);
	lTalon2 = new CANTalon(lTal2);

	rEnc = new Encoder(rEncA, rEncB);
	lEnc = new Encoder(lEncA, lEncB);
	//rEnc->SetDistancePerPulse(0.04477);
	//lEnc->SetDistancePerPulse(0.04477);
	panOut = new DriveOut();
	this->gyro = gyro;
	lEnc->SetReverseDirection(true);
	driveIn = new DriveIn(rEnc, lEnc);
	driveOut = new DriveOut();
	drive_controller = new PIDController(RDRIVE_CONTROLLER_P, RDRIVE_CONTROLLER_I, RDRIVE_CONTROLLER_D, driveIn, driveOut);

	drive_controller->SetOutputRange(-.75, .75);
	angleIn = new AngleIn(gyro);
	angleOut = new DriveOut();
	//enableReal();
	if (real) {
		angle_controller = new PIDController(RANGLE_CONTROLLER_P, RANGLE_CONTROLLER_I, RANGLE_CONTROLLER_D, angleIn, angleOut);
	} else {
		angle_controller = new PIDController(PANGLE_CONTROLLER_P, PANGLE_CONTROLLER_I, PANGLE_CONTROLLER_D, angleIn, angleOut);
	}
	angle_controller->SetOutputRange(-.5, .5);
//	pan = panIn;
	pan = new AngleIn(gyro);
	pan_controller = new PIDController( /*0.0512,0,0.08,*/ .0441, 0, 0.13 ,  pan, panOut);
	// 0.0729007 5 degree
	// 0.143002  2 degree
	// 	0.0573003 10 degree
	//  0.0346          15
//	pan_controller->SetOutputRange(-.4, .4);
	rEnc->Reset();
	lEnc->Reset();
	//drive_controller->Enable();
}

void DriveModule::setRightPower(double rPow) {
	rTalon1->Set(rPow);
	rTalon2->Set(rPow);
}

void DriveModule::calibrate() {
	gyro->ZeroYaw();
}

void DriveModule::setLeftPower(double lPow) {
	lTalon1->Set(lPow);
	lTalon2->Set(lPow);
}

void DriveModule::EnablePID(bool enable) {
	if(enable){
		drive_controller->Enable();

		angle_controller->Enable();
	} else {
		drive_controller->Disable();
		angle_controller->Disable();
	}
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
	setLeftPower(-leftMotorOutput);
	setRightPower(rightMotorOutput);
}

void DriveModule::driveTank(double lPow, double rPow) {
	setLeftPower(-lPow);
	setRightPower(rPow);
}

double DriveModule::getDriveOutput() {
	return driveOut->getPower();
}

double DriveModule::getAngleOutput() {
	return angleOut->getPower();
}

void DriveModule::setDriveSetpoint(double setpoint) {
	drive_controller->SetSetpoint(setpoint*TICKS_PER_INCH);
}

double DriveModule::getDriveSetpoint() {
	return drive_controller->GetSetpoint();
}

void DriveModule::setAngleSetpoint(double angle) {
	angle_controller->SetSetpoint(angle);
}

double DriveModule::getAngleSetpoint() {
	return angle_controller->GetSetpoint();
}

double DriveModule::getRightEncoder() {
	return rEnc->PIDGet();
}

double DriveModule::getLeftEncoder() {
	return lEnc->PIDGet();
}

double DriveModule::getP() {
	return drive_controller->GetP();
}

double DriveModule::getI() {
	return drive_controller->GetI();

}

double DriveModule::getD() {
	return drive_controller->GetD();
}

void DriveModule::setPID(double p, double i, double d) {
	angle_controller->SetPID(p,i,d);
}

void DriveModule::setMaxPower(double min, double max) {
	drive_controller->SetOutputRange(min,max);
	angle_controller->SetOutputRange(min, max);
}
void DriveModule::drive(double setpoint) {
	rEnc->Reset();
	lEnc->Reset();
	Timer* time = new Timer();
	time->Start();
	EnablePID(true);
	setDriveSetpoint(setpoint);
	setAngleSetpoint(0);
	//setAngleSetpoint(0);
	std::cout << " in " <<std::endl;
	while(time->Get() < 5 && abs(driveIn->PIDGet() - getDriveSetpoint()) > 2) {
		std::cout << getRightEncoder() <<std::endl;
		driveTank(-driveOut->getPower() - angleOut->getPower(), -driveOut->getPower() + angleOut->getPower());
	}
	//rEnc->Reset();
	//lEnc->Reset();
	EnablePID(false);
	driveTank(0,0);
}
void DriveModule::driveWithoutAngle(double setpoint) {
	rEnc->Reset();
	lEnc->Reset();
	Timer* time = new Timer();
	time->Start();
	EnablePID(true);
	setDriveSetpoint(setpoint);
	setAngleSetpoint(0);
	//setAngleSetpoint(0);
	std::cout << " in " <<std::endl;
	while(time->Get() < 5 && abs(driveIn->PIDGet() - getDriveSetpoint()) > 2) {
		std::cout << getRightEncoder() <<std::endl;
		driveTank(-driveOut->getPower(), -driveOut->getPower());
	}
	//rEnc->Reset();
	//lEnc->Reset();
	EnablePID(false);
	driveTank(0,0);
}

double DriveModule::getAngle() {
	return gyro->GetYaw();
}

void DriveModule::turn(double angle) {
	rEnc->Reset();
	lEnc->Reset();
	Timer* time = new Timer();
	time->Start();
	EnablePID(true);
	setAngleSetpoint(angle);
	std::cout << abs(angleIn->PIDGet() - getAngleSetpoint()) <<std::endl;
	while(time->Get() < 4 && (((angleIn->PIDGet() - getAngleSetpoint()) > 1) || ((angleIn->PIDGet() - getAngleSetpoint()) < -1))) {
		//std::cout <<  abs(angleIn->PIDGet() - getAngleSetpoint())  <<std::endl;
		driveTank(-angleOut->getPower(), angleOut->getPower());
	}
	//	rEnc->Reset();
	//	lEnc->Reset();
	EnablePID(false);
	driveTank(0,0);
}

void DriveModule::turnALPHA(double angle) {
	Timer* time = new Timer();
	time->Start();
	EnablePID(true);
	setAngleSetpoint(angle);
	std::cout << abs(angleIn->PIDGet() - getAngleSetpoint()) <<std::endl;
	while(time->Get() < 4 && (((angleIn->PIDGet() - getAngleSetpoint()) > 1) || ((angleIn->PIDGet() - getAngleSetpoint()) < -1))) {
		//std::cout <<  abs(angleIn->PIDGet() - getAngleSetpoint())  <<std::endl;
		driveTank(-angleOut->getPower(), angleOut->getPower());
	}
	rEnc->Reset();
	lEnc->Reset();
	EnablePID(false);
	driveTank(0,0);
}
void DriveModule::setPanPID(double p, double i, double d) {
	pan_controller->SetPID(p, i , d);
}
void DriveModule::setDrivePID(double p, double i, double d) {
	drive_controller->SetPID(p, i , d);
}
double DriveModule::getPanP() {
	return pan_controller->GetP();
}

double DriveModule::getPanI() {
	return pan_controller->GetI();
}

double DriveModule::getPanD() {
	return pan_controller->GetD();
}

void DriveModule::setPanSetpoint(double setPoint) {
	pan_controller->SetSetpoint(setPoint);
}

void DriveModule::enablePan(bool enable) {
	if(enable)
		pan_controller->Enable();
	else
		pan_controller->Disable();
}

double DriveModule::getPanOutput() {
	return panOut->getPower();
}

double DriveModule::getPanSetpoint() {
	return pan_controller->GetSetpoint();
}

double DriveModule::getPanInput() {
	return pan->PIDGet();
}

void DriveModule::reset() {
	gyro->Reset();
}

void DriveModule::resetEncoders() {
	rEnc->Reset();
	lEnc->Reset();
}
