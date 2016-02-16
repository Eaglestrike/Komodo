/*
 * ShooterModule.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */

#include <modules/ShooterModule.h>



ShooterModule::ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport, int solenoidPort, int buttonport) {
	// TODO Auto-generated constructor stub
	angle = new AnalogPotentiometer(anglePort);
	angleMotor = new CANTalon(angleMotorPort);
	leftShooter = new CANTalon(leftport);

	rightShooter = new CANTalon(rightport);
	shooterSol = new Solenoid(solenoidPort);
	button = new DigitalInput(buttonport);
	leftShooter->EnableControl();
	leftShooter->SetControlMode(CANSpeedController::kSpeed);

	shootIn = new ShooterIn(angle);
	shootOut = new ShooterOut();
	angleController = new PIDController(RSHOOTER_CONTROLLER_P, RSHOOTER_CONTROLLER_I, RSHOOTER_CONTROLLER_D,  shootIn, angleMotor);
}

ShooterModule::~ShooterModule() {
	// TODO Auto-generated destructor stub
}

double ShooterModule::getAngle()  {
	return angle->Get();
}

bool ShooterModule::isBallIn() {
	return !button->Get();
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
void ShooterModule::speed(double speed){
	leftShooter->Set(speed);
}
double ShooterModule::getSpeed(){
	return leftShooter->GetSpeed();
}
void ShooterModule::setPID(double p, double i, double d){
	angleController->SetPID(p,i,d);
}
void ShooterModule::setShooterSpeed(float speed){

}
double ShooterModule::getP(){
	return angleController->GetP();
}
double ShooterModule::getI(){
	return angleController->GetI();
}
double ShooterModule::getD(){
	return angleController->GetD();
}
void ShooterModule::enablePID(){
	angleController->Enable();
}
void ShooterModule::setMaxPower(double power){
	angleController->SetOutputRange(-power, power);
}
double ShooterModule::getSetpoint(){
	return angleController->GetSetpoint();
}

void ShooterModule::setLeftShooterPID(double p, double i, double d) {
	leftShooter->SetPID(p,i,d);
}

double ShooterModule::getShooterSetpoint() {
	return leftShooter->GetSetpoint();
}

double ShooterModule::getShooterP() {
	return leftShooter->GetP();
}

double ShooterModule::getShooterI() {
	return leftShooter->GetI();
}

double ShooterModule::getShooterD() {
	return leftShooter->GetD();
}
