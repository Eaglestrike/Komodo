/*
 * ShooterModule.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: Samsung
 */

#include <modules/ShooterModule.h>



ShooterModule::ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport, int solenoidPort, int buttonport) : RobotModule("ShooterModule") {
	// TODO Auto-generated constructor stub
	angle = new AnalogPotentiometer(anglePort);
	angleMotor = new WPI_TalonSRX(angleMotorPort);
	leftShooter = new WPI_TalonSRX(leftport);

	rightShooter = new WPI_TalonSRX(rightport);
	shooterSol = new Solenoid(solenoidPort);
	button = new DigitalInput(buttonport);
	leftShooter->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
	//leftShooter->EnableControl();
	//leftShooter->SetInverted(true);
	//	leftShooter->SetPosition(0);
	//	leftShooter->SetControlMode(CANSpeedController::kSpeed);
	//	leftShooter->SetPID(.12996,0,3);
	//
	//	rightShooter->SetFeedbackDevice(TalonSRX::QuadEncoder);
	//	//rightShooter->EnableControl();
	//	rightShooter->SetPosition(0);
	//	rightShooter->SetControlMode(CANSpeedController::kSpeed);
	//	rightShooter->SetPID(.12996,0,3);
	real = true;
	shootIn = new ShooterIn(angle);
	shootOut = new ShooterOut();
	if (real) {
		angleController = new PIDController(RSHOOTER_CONTROLLER_P, RSHOOTER_CONTROLLER_I, RSHOOTER_CONTROLLER_D,  shootIn, angleMotor);
	} else {
		angleController = new PIDController(PSHOOTER_CONTROLLER_P, PSHOOTER_CONTROLLER_I, PSHOOTER_CONTROLLER_D,  shootIn, angleMotor);
	}
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
	leftShooter->Set(ControlMode::PercentOutput, -left);
	rightShooter->Set(ControlMode::PercentOutput, right);
	Wait(time);
	leftShooter->Set(ControlMode::PercentOutput, 0);
	rightShooter->Set(ControlMode::PercentOutput, 0);
	shooterSol->Set(false);
	shot = false;
}

void ShooterModule::shootKicker(bool kick) {
	shooterSol->Set(kick);
}

void ShooterModule::mShoot(double power) {
	rightShooter->Set(ControlMode::PercentOutput, power);
	leftShooter->Set(ControlMode::PercentOutput, -power);

}
void ShooterModule::setAngleMotorPower(double power) {
	angleMotor->Set(ControlMode::PercentOutput, power);
}

void ShooterModule::tilt(double angle) {
	if(real) {
		if(angle > RMAXIMUM_ANGLE) {
			angle = RMAXIMUM_ANGLE;
		}
		if(angle < RMINIMUM_ANGLE) {
			angle = RMINIMUM_ANGLE;
		}
	}
	if(!real) {
		if(angle > PMAXIMUM_ANGLE) {
			angle = PMAXIMUM_ANGLE;
		}
		if(angle < PMINIMUM_ANGLE) {
			angle = PMINIMUM_ANGLE;
		}
	}
	angleController->SetSetpoint(angle);
}
void ShooterModule::speed(double speed){
	rightShooter->Set(ControlMode::PercentOutput, speed);
}
double ShooterModule::getSpeed(){
	return rightShooter->GetSelectedSensorVelocity(0);
}
void ShooterModule::setPID(double p, double i, double d){
	angleController->SetPID(p,i,d);
}
void ShooterModule::setShooterSpeed(double speed){
	rightShooter->Set(ControlMode::PercentOutput, speed);
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
void ShooterModule::disablePID(){
	angleController->Disable();
}
void ShooterModule::setMaxPower(double power){
	angleController->SetOutputRange(-power, power);
}
double ShooterModule::getSetpoint(){
	return angleController->GetSetpoint();
}

