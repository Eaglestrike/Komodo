#include "WPILib.h"
#include <math.h>
#include "Xbox.h"
#include "modules/IntakeModule.h"
#include "modules/DriveModule.h"
#include "Peripherals/AutonomousCode/LogisticFunction.h"
#include <iostream>
#include <cstdio>
#include <ctime>

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
	LogisticFunction* func;

	Joystick* rJoy;
	Joystick* lJoy;
	Xbox* controller;

	void RobotInit()
	{
		rJoy = new Joystick(1);
		lJoy = new Joystick(0);
		controller = new Xbox(2);
		intake = new IntakeModule(4);
		drive = new DriveModule(DRIVE_LEFT1, DRIVE_LEFT2, DRIVE_RIGHT1, DRIVE_RIGHT2, DRIVE_ENCODER_1_A, DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B);
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

	void autonGo(double distance, double time) {//Time in seconds for now
		func = new LogisticFunction(time, distance);
		std::cout << "Starting timer" << std::endl;
		Timer timer;
		timer.Start();
		std::cout << "Starting function finding" << std::endl;
		while (timer.Get() < time) { //&& func->getDistance(timer.Get()-5 < distance) {
			drive->setDriveSetpoint(func->getDistance(timer.Get()));
			drive->setLeftPower(drive->getDriveOutput());
			drive->setRightPower(drive->getDriveOutput());
			std::cout << drive->getDriveOutput() << std::endl;
		}
//		std::cout << func->getA() << std::endl;
//		std::cout << func->getDistance(2) << std::endl;
		std::cout << "Finishing functions" << std::endl;
	}

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
		//arcade = false;
		std::cout << "Autonomous starting" << std::endl;
		autonGo(900,2);
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

		intake->setPower(controller->getLY());
	}

	void TestInit() {

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
