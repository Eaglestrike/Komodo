#include "WPILib.h"
#include "NetworkTables/NetworkTable.h"
//#include "HUDServer.h"
#include "Xbox.h"

class Robot: public IterativeRobot
{
public:
	std::shared_ptr<NetworkTable> visionTable;
private:
	double movementInX=0;
	Xbox* xbox;

	double movementInY=0;
	double x;
	double y;
	LiveWindow *lw = LiveWindow::GetInstance();
	double frameWidth,frameHeight,xMovePerTick,yMovePerTick,tolerance;
	int movementFactor=1;
	Servo* up;
	Servo* side;
	//HUDServer* server;

	void RobotInit()
	{
		std::cout << "start" << std::endl;
		xbox = new Xbox(2);
		visionTable = NetworkTable::GetTable("visionTable");
		std::cout << "hello" << std::endl;
		up = new Servo(0);
		side = new Servo(1);
		std::cout << "hello" << std::endl;
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
		movementInX=0;
		movementInY=0;
		std::cout<<visionTable->GetNumber("detectedObject")<<std::endl;
		if(visionTable->GetNumber("detectedObject")==1){
			std::cout<<"Saw an object"<<std::endl;

			x = visionTable->GetNumber("X");
			y = visionTable->GetNumber("Y");

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
		std::cout<<"movementInX: "<<movementInX<<" movementInY: "<<movementInY<<std::endl;
		std::cout<<"Current Y Angle: "<<up->GetAngle()<<" Current X Angle: "<<side->GetAngle()<<std::endl;
		std::cout<<"X: "<<x<<" Y: "<<y<<std::endl;
	}

	void TestPeriodic()
	{

		lw->Run();
		if(xbox->getA()){
			up->SetAngle(up->GetAngle()-1);
		}
		if(xbox->getY()){
			up->SetAngle(up->GetAngle()+1);
		}
		if(xbox->getB()){
			up->SetAngle(side->GetAngle()+1);
		}
		if(xbox->getX()){
			up->SetAngle(side->GetAngle()-1);
		}
		std::cout<<"Current Y Angle: "<<up->GetAngle()<<" Current X Angle: "<<side->GetAngle()<<std::endl;

	}
};

START_ROBOT_CLASS(Robot)
