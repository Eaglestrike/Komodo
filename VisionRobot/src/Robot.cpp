#include "WPILib.h"
#include "NetworkTables/NetworkTable.h"
//#include "HUDServer.h"
#include "Xbox.h"
#include <math.h>
class Robot: public IterativeRobot
{
public:
	std::shared_ptr<NetworkTable> visionTable;
private:
	double movementInX=0;
	double xTime;
	double yTime;
	Xbox* xbox;
	double xAngle;
	Timer* xTimer;
	Timer* yTimer;
	double yAngle;
	int ticks=0;
	double movementInY=0;
	double x;
	double y;
	LiveWindow *lw = LiveWindow::GetInstance();
	double frameWidth,frameHeight,xMovePerTick,yMovePerTick,Xtolerance, Ytolerance;
	int movementFactor=1;
	Servo* up;
	Servo* side;
	//HUDServer* server;

	void RobotInit()
	{
		xbox = new Xbox(2);
		visionTable = NetworkTable::GetTable("visionTable");
		up = new Servo(0);
		side = new Servo(1);
		xTimer = new Timer();
		yTimer =  new Timer();
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
		Xtolerance = visionTable->GetNumber("XTolerance");
		Ytolerance = visionTable->GetNumber("YTolerance");
		yMovePerTick = visionTable->GetNumber("yTicks");
		xMovePerTick = visionTable->GetNumber("xTicks");
		xTimer->Reset();
		yTimer->Reset();
		xTimer->Start();
		yTimer->Start();
		up->SetAngle(60);
		side->SetAngle(0);
	}

	void TeleopPeriodic()
	{

		Xtolerance = visionTable->GetNumber("XTolerance");
		Ytolerance = visionTable->GetNumber("YTolerance");

		movementInX=0;
		movementInY=0;
		std::cout<<visionTable->GetNumber("detectedObject")<<std::endl;
		if(visionTable->GetNumber("detectedObject")==1){
			std::cout<<"ticks: "<<ticks<<std::endl;

			x = visionTable->GetNumber("X");
			y = visionTable->GetNumber("Y");

//			if(x>(frameWidth/2+Xtolerance)){
//				movementInX = -xMovePerTick;
//			}
//			if(x<(frameWidth/2-Xtolerance)){
//				movementInX = xMovePerTick;
//			}
//			if(y>(frameHeight/2+Ytolerance)){
//				movementInY=yMovePerTick;
//			}
//			else if(y<(frameHeight/2-Ytolerance)){
//				movementInY=-yMovePerTick;
//			}
			//0.19sec/60degrees move speed on servos
			if(xTimer->Get()>=xTime){
				xAngle = visionTable->GetNumber("xAngle");
				double moveAngle = xAngle-26.4;
				side->SetAngle(side->GetAngle()+moveAngle);
				xTime = abs(double((.19/60)*moveAngle));
				xTimer->Reset();
				xTimer->Start();
			}
			if(yTimer->Get()>=yTime){
				yAngle = visionTable->GetNumber("yAngle");
				double moveAngle = yAngle-26.4;
				up->SetAngle(up->GetAngle()+moveAngle);
				yTime = abs(double((.19/60)*moveAngle));
				yTimer->Reset();
				yTimer->Start();
			}
			if(up->GetAngle()>90){
				up->SetAngle(90);
			}
			ticks++;
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

			side->SetAngle(side->GetAngle()+xMovePerTick*movementFactor*2);
		}
//		std::cout<<"movementInX: "<<movementInX<<" movementInY: "<<movementInY<<std::endl;
//		std::cout<<"Current Y Angle: "<<up->GetAngle()<<" Current X Angle: "<<side->GetAngle()<<std::endl;
//		std::cout<<"X: "<<x<<" Y: "<<y<<std::endl;
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
			side->SetAngle(side->GetAngle()+1);
		}
		if(xbox->getX()){
			side->SetAngle(side->GetAngle()-1);
		}
		std::cout<<"Current Y Angle: "<<up->GetAngle()<<" Current X Angle: "<<side->GetAngle()<<std::endl;

	}
};

START_ROBOT_CLASS(Robot)
