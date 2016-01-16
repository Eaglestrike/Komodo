#include "WPILib.h"
#include "NetworkTables/NetworkTable.h"
//#include "HUDServer.h"

class Robot: public IterativeRobot
{
public:
	std::shared_ptr<NetworkTable> visionTable;
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	double frameWidth,frameHeight,xMovePerTick,yMovePerTick,tolerance;
	int movementFactor=1;
	Servo* up;
	Servo* side;
	//HUDServer* server;

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
		up->SetAngle(0);
		side->SetAngle(0);
	}

	void TeleopPeriodic()
	{
		std::cout<<visionTable->GetNumber("detectedObject")<<std::endl;
		if(visionTable->GetNumber("detectedObject")==1){
			std::cout<<"Saw an object"<<std::endl;

			int x = visionTable->GetNumber("X");
			int y = visionTable->GetNumber("Y");
			double movementInX=0;
			double movementInY=0;
			if(x>frameWidth/2+tolerance){
				movementInX = xMovePerTick;
			}
			if(x<frameWidth/2-tolerance){
				movementInX = -xMovePerTick;
			}
			if(y>frameWidth/2+tolerance){
				movementInY=-yMovePerTick;
			}
			if(y<frameWidth/2-tolerance){
				movementInY=yMovePerTick;
			}
			std::cout<<"movementInX: "<<movementInX<<" movementInY: "<<movementInY<<std::endl;
			std::cout<<"Current Y Angle: "<<up->GetAngle()<<" Current X Angle: "<<side->GetAngle()<<std::endl;
			std::cout<<"X: "<<x<<" Y: "<<y<<std::endl;

			up->SetAngle(up->GetAngle()-movementInY);
			side->SetAngle(side->GetAngle()-movementInX);
			if(up->GetAngle()>90){
				up->SetAngle(90);
			}
		}
		else{

			std::cout<<"Sweeping"<<std::endl;
			if(side->GetAngle()==180){
				movementFactor=-1;
			}
			if(side->GetAngle()==0){
				movementFactor=1;
			}
			std::cout<<side->GetAngle()<<std::endl;

			side->SetAngle(side->GetAngle()+xMovePerTick*movementFactor);
		}
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
