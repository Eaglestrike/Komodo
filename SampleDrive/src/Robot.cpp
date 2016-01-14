#include "WPILib.h"
#include <math.h>
#include "Xbox.h"
#include "modules/IntakeModule.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	bool arcade;
	double throttle;
	double angle;
	double leftMotorOutput;
	double rightMotorOutput;
	CANTalon* rTalon1;
	CANTalon* rTalon2;
	CANTalon* lTalon1;
	CANTalon* lTalon2;

	IntakeModule* intake;

	Joystick* rJoy;
	Joystick* lJoy;
	Xbox* controller;

	void RobotInit()
	{
		rJoy = new Joystick(1);
		lJoy = new Joystick(0);
		lTalon1 = new CANTalon(0);
		lTalon2 = new CANTalon(1);
		rTalon1 = new CANTalon(2);
		rTalon2 = new CANTalon(3);
		controller = new Xbox(2);
		intake = new IntakeModule(4);
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
			throttle = rJoy->GetY();
			angle = lJoy->GetX();
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
			lTalon1->Set(leftMotorOutput);
			lTalon2->Set(leftMotorOutput);
			rTalon1->Set(rightMotorOutput);
			rTalon2->Set(rightMotorOutput);

		} else {
			lTalon1->Set(lJoy->GetY());
			lTalon2->Set(lJoy->GetY());
			rTalon1->Set(rJoy->GetY());
			rTalon2->Set(rJoy->GetY());
		}

		intake->setPower(controller->getLY());
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
