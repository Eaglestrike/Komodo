#include "WPIlib.h"
#include <math.h>
#include "Xbox.h"
#include "modules/IntakeModule.h"
#include "modules/DriveModule.h"
//#include "Peripherals/AutonomousCode/LogisticFunction.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include "WPILib.h"
#include "modules/ShooterModule.h"
#include "modules/DriveModule.h"
#include "Settings.h"
#include "modules/FlipperModule.h"
#include "Xbox.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	bool arcade;

	IntakeModule* intake;
	DriveModule* drive;
	//LogisticFunction* func;
	ShooterModule* shooter;
	Joystick* rJoy;
	Joystick* lJoy;
	int intakeCounter = 0;
	Xbox* controller;
	bool intakes = false;
	//Solenoid* intakeSolenoid;
	FlipperModule* tomahawks;
	//PCM* pcm;

	void RobotInit()
	{
		rJoy = new Joystick(1);
		lJoy = new Joystick(0);
		controller = new Xbox(2);
		intake = new IntakeModule(INTAKE_MOTOR, INTAKE_SOL);
		//intakeSolenoid = new Solenoid(0);
		drive = new DriveModule(DRIVE_LEFT1, DRIVE_LEFT2, DRIVE_RIGHT1, DRIVE_RIGHT2, DRIVE_ENCODER_1_A, DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B);
		shooter = new ShooterModule(0, ANGLEMOTOR, SHOOTERMOTOR1, SHOOTERMOTOR2, SHOOTER_SOL);
		shooter->createThread();
		//tomahawks = new FlipperModule(TOMOHAWKS);
		//pcm = new PCM();
	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		std::cout << "Starting auton" << std::endl;
		autoSelected = *((std::string*)chooser->GetSelected());
		std::string autoSelectedString = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelectedString << std::endl;
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	//	void autonGo(double distance, double time) {//Time in seconds for now
	//		func = new LogisticFunction(time, distance);
	//		std::cout << "Starting timer" << std::endl;
	//		Timer timer;
	//		timer.Start();
	//		std::cout << "Starting function finding" << std::endl;
	//		while (timer.Get() < time) { //&& func->getDistance(timer.Get()-5 < distance) {
	//			drive->setDriveSetpoint(func->getDistance(timer.Get()));
	//			drive->setLeftPower(drive->getDriveOutput());
	//			drive->setRightPower(drive->getDriveOutput());
	//			std::cout << drive->getDriveOutput() << std::endl;
	//		}
	////		std::cout << func->getA() << std::endl;
	////		std::cout << func->getDistance(2) << std::endl;
	//		std::cout << "Finishing functions" << std::endl;
	//	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{
		arcade = false;
		intake->deployIntake();
		//std::cout << "Autonomous starting" << std::endl;
		//autonGo(900,2);
	}

	void TeleopPeriodic()
	{
		if (rJoy->GetRawButton(1)) {
			arcade = !arcade;
		}

		if (arcade) {
			drive->driveArcade(rJoy->GetY(),lJoy->GetX());
		} else {
			drive->driveTank(lJoy->GetY(), rJoy->GetY());
		}

		intake->setSpinPower(controller->getLY());
		//if(rJoy->GetTrigger() && lJoy->GetTrigger() && !shooter->getShot()) {
		//shooter->shoot(1, 1, 2);
		//}
		if(controller->getX()) {
			shooter->shootKicker(true);
		}
		else {
			shooter->shootKicker(false);
		}
		shooter->mShoot(controller->getLX());
		shooter->setAngleMotorPower(.1 * controller->getRX());

//		if(controller->getY() != intakes){
//			if(intakes==0){
//				intake->deployIntake();
//			}
//			else if(intakes==1){
//				intake->retractIntake();
//			}
//		}
//
//		intakes = controller->getY();
	//	if(controller->getA()) {
			//intakeSolenoid->Set(true);
			//tomahawks->Deploy();
//		}


//		if(intakeCounter % 2 == 0){
//			intake->retractIntake();
//		}else if(intakeCounter % 4 == 0){
//			intake->deployIntake();
//		}
		if(intakeCounter % 20 == 0) {
			std::cout << "controller value" << controller->getA() << std::endl;
			//std::cout << "value of solenoid" << intakeSolenoid->Get() << std::endl;

		}
		intakeCounter++;

	}

	void TestInit() {

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
