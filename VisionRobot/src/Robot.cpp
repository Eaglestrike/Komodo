#include "WPILib.h"
//#include "HUDServer.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	int frameWidth,frameHeight,xMovePerTick,yMovePerTick,tolerance;
	int movementFactor=1;
	Servo* up;
	Servo* side;
	//HUDServer* server;
	NetworkTable* visionTable;
	void RobotInit()
	{
		visionTable = NetworkTable::GetTable("visionTable");
		up = new Servo(0);
		side = new Servo(1);
		//server = new HUDServer(500, up, side);

	}



	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{
		frameWidth = visionTable->GetNumber("width");
		frameHeight = visionTable->GetNumber("height");
		tolerance = visionTable->GetNumber("Tolerance");
		yMovePerTick = visionTable->GetNumber("yTicks");
		xMovePerTick = visionTable->GetNumber("xTicks");
	}

	void TeleopPeriodic()
	{
		if(visionTable->GetBoolean("detectedObject")){
			std::cout<<"Saw an object"<<std::endl;
			int x = visionTable->GetNumber("X");
			int y = visionTable->GetNumber("Y");
			int movementInX  = (x>frameWidth/2+tolerance)?(xMovePerTick):(0);
			movementInX = (x<frameWidth/2-tolerance)?(-xMovePerTick):(0);
			int movementInY  = (y>frameHeight/2+tolerance)?(-yMovePerTick):(0);
			movementInY = (y<frameHeight/2-tolerance)?(yMovePerTick):(0);
			up->SetAngle(up->GetAngle()+movementInY);
			side->SetAngle(side->GetAngle()+movementInX);
		}
		else{

			std::cout<<"Sweeping"<<std::endl;
			if(side->GetAngle()>180){
				movementFactor=1;
			}
			if(side->GetAngle()<0){
				movementFactor=0;
			}


			side->SetAngle(side->GetAngle()+xMovePerTick*movementFactor);
		}
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
