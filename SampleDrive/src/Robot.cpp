#include "WPILib.h"
#include <math.h>
#include "Xbox.h"
#include "modules/IntakeModule.h"
#include "modules/DriveModule.h"

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

	Joystick* rJoy;
	Joystick* lJoy;
	Xbox* controller;

	void RobotInit()
	{
		rJoy = new Joystick(1);
		lJoy = new Joystick(0);
		controller = new Xbox(2);
		intake = new IntakeModule(4);
		drive = new DriveModule(0,1,2,3,4,5,6,7);
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
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
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
		arcade = false;
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

	void TestPeriodic()
	{
		lw->Run();
	}

	double expDrive(double totalDistance, double totalTime, double currentTime) {

	}
};

START_ROBOT_CLASS(Robot)
