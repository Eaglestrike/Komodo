/*
 * DriveModule.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: natek
 */
#include "DriveModule.h"

DriveModule::DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB, PIDSource*  panIn) {
	rTalon1 = new CANTalon(rTal1);
	rTalon2 = new CANTalon(rTal2);
	lTalon1 = new CANTalon(lTal1);
	lTalon2 = new CANTalon(lTal2);

	rEnc = new Encoder(rEncA, rEncB);
	lEnc = new Encoder(lEncA, lEncB);
	//rEnc->SetDistancePerPulse(0.04477);
	//lEnc->SetDistancePerPulse(0.04477);
	panOut = new DriveOut();
	lEnc->SetReverseDirection(true);
	driveIn = new DriveIn(rEnc, lEnc);
	driveOut = new DriveOut();
	drive_controller = new PIDController(DRIVE_CONTROLLER_P, DRIVE_CONTROLLER_I, DRIVE_CONTROLLER_D, driveIn, driveOut);
	drive_controller->SetOutputRange(-.5, .5);
	angleIn = new AngleIn(rEnc, lEnc);
	angleOut = new DriveOut();
	angle_controller = new PIDController(ANGLE_CONTROLLER_P, ANGLE_CONTROLLER_I, ANGLE_CONTROLLER_D, angleIn, angleOut);
	angle_controller->SetOutputRange(-.5, .5);
	pan = panIn;
	pan_controller = new PIDController(0.005,0,.84, pan, panOut);
	pan_controller->SetOutputRange(-.4, .4);
	//drive_controller->Enable();
}

void DriveModule::setRightPower(double rPow) {
	rTalon1->Set(rPow);
	rTalon2->Set(rPow);
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
	drive_controller->SetSetpoint(setpoint);
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
	return angle_controller->GetP();
}

double DriveModule::getI() {
	return angle_controller->GetI();

}

double DriveModule::getD() {
	return angle_controller->GetD();
}

void DriveModule::setPID(double p, double i, double d) {
	angle_controller->SetPID(p,i,d);
}

void DriveModule::drive(double setpoint) {
	rEnc->Reset();
	lEnc->Reset();
	Timer* time = new Timer();
	time->Start();
	EnablePID(true);
	setDriveSetpoint(setpoint);
	std::cout << " in " <<std::endl;
	while(time->Get() < 4 && abs(driveIn->PIDGet() - getDriveSetpoint()) > 1) {
		std::cout << driveOut->getPower() <<std::endl;
		driveTank(driveOut->getPower(), driveOut->getPower());
	}
	rEnc->Reset();
	lEnc->Reset();
	EnablePID(false);
	driveTank(0,0);
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
		std::cout <<  abs(angleIn->PIDGet() - getAngleSetpoint())  <<std::endl;
		driveTank(angleOut->getPower(), -angleOut->getPower());
	}
	rEnc->Reset();
	lEnc->Reset();
	EnablePID(false);
	driveTank(0,0);
}

void DriveModule::setPanPID(double p, double i, double d) {
	pan_controller->SetPID(p, i , d);
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
