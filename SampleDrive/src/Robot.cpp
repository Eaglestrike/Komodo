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
public:
	std::shared_ptr<NetworkTable> visionTable;
private:
	double movementInX=0;
	double xTime;
	double yTime;
	double xAngle;
	double setpoint=0;
	Compressor* test;
	double yAngle;
	int ticks=0;
	int yticks=0;
	double movementInY=0;
	double x;
	double y;
	LiveWindow *lw = LiveWindow::GetInstance();
	double frameWidth,frameHeight,xMovePerTick,yMovePerTick,Xtolerance, Ytolerance;
	int movementFactor=1;
	Servo* up;
	Servo* side;


	bool arcade;

	IntakeModule* intake;
	DriveModule* drive;
	//LogisticFunction* func;
	ShooterModule* shooter;
	Joystick* rJoy;
	Joystick* lJoy;
	int intakeCounter = 0;
	Xbox* controller;
	bool intakes = true;
	bool tomah = false;
	//Solenoid* intakeSolenoid;
	FlipperModule* tomahawks;
	int tomahawkCounter = 0;
	//PCM* pcm;
	int counter=0;


	void RobotInit()
	{
		visionTable = NetworkTable::GetTable("visionTable");
		test = new Compressor(0);
		up = new Servo(0);
		side = new Servo(1);
		rJoy = new Joystick(1);
		lJoy = new Joystick(0);
		controller = new Xbox(2);
		intake = new IntakeModule(INTAKE_MOTOR, INTAKE_SOL);
		//intakeSolenoid = new Solenoid(0);
		drive = new DriveModule(DRIVE_LEFT1, DRIVE_LEFT2, DRIVE_RIGHT1, DRIVE_RIGHT2, DRIVE_ENCODER_1_A, DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B);
		shooter = new ShooterModule(POT, ANGLEMOTOR, SHOOTERMOTOR1, SHOOTERMOTOR2, SHOOTER_SOL, 9);
		shooter->createThread();
		tomahawks = new FlipperModule(TOMOHAWKS);
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

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

		std::cout<<"yoyo"<<std::endl;
		frameWidth = visionTable->GetNumber("width");
		frameHeight = visionTable->GetNumber("height");
		Xtolerance = visionTable->GetNumber("XTolerance");
		Ytolerance = visionTable->GetNumber("YTolerance");
		yMovePerTick = visionTable->GetNumber("yTicks");
		xMovePerTick = visionTable->GetNumber("xTicks");

		up->SetAngle(60);
		side->SetAngle(0);
		arcade = false;
		std::cout<<"yoyoyo"<<std::endl;
		shooter->enablePID();
		shooter->setMaxPower(.5);
		shooter->tilt(RSHOOT_ANGLE);
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
		//shooter->setAngleMotorPower(.2 * controller->getRX());

		if(controller->getY() != intakes){
			intakeCounter++;
		}
		intakes = controller->getY();

		if(controller->getA() != tomah){
			tomahawkCounter++;
		}
		if(controller->getLB()){
			shooter->tilt(RINTAKE_ANGLE);
		}
		if(controller->getRB()){
			shooter->tilt(RSHOOT_ANGLE);
		}
		if(controller->getStart()){
			shooter->tilt(.3);
		}
		if(controller->getBack()) {
			shooter->tilt(LEVEL_ANGLE);
		}

		if(controller->getRB()) {
			shooter->mShoot(.608);
			Wait(2);
			shooter->shootKicker(true);
			Wait(.5);
			shooter->mShoot(0);
		}
		tomah = controller->getA();


		//		if(controller->getA() != tomah){
		//			if(tomah==0){
		//				tomahawks->Deploy();
		//			}
		//			else if(tomah==1){
		//				tomahawks->Retract();
		//			}
		//		}
		//		if(controller->getA()) {
		//			//intakeSolenoid->Set(true);
		//			tomahawks->Deploy();
		//		}

		if(tomahawkCounter % 4 == 0) {
			tomahawks->Retract();
		}
		else if(tomahawkCounter % 2 == 0) {
			tomahawks->Deploy();
		}

		if(intakeCounter % 4 == 0){
			intake->retractIntake();
		}else if(intakeCounter % 2 == 0){
			intake->deployIntake();
		}
		if(intakeCounter % 20 == 0) {
			std::cout << "controller value" << controller->getA() << std::endl;
			//std::cout << "value of solenoid" << intakeSolenoid->Get() << std::endl;

		}
		//intakeCounter++;
		std::cout<<"Yo"<<std::endl;
		Xtolerance = visionTable->GetNumber("XTolerance");
		Ytolerance = visionTable->GetNumber("YTolerance");

		movementInX=0;
		movementInY=0;
		std::cout<<visionTable->GetNumber("detectedObject")<<std::endl;
		if(visionTable->GetNumber("detectedObject")==1){
			std::cout<<"ticks: "<<ticks<<std::endl;

			x = visionTable->GetNumber("X");
			y = visionTable->GetNumber("Y");

			if(x>(frameWidth/2+Xtolerance)){
				movementInX = -xMovePerTick;
			}
			if(x<(frameWidth/2-Xtolerance)){
				movementInX = xMovePerTick;
			}
			if(y>(frameHeight/2+Ytolerance)){
				movementInY=yMovePerTick;
			}
			else if(y<(frameHeight/2-Ytolerance)){
				movementInY=-yMovePerTick;
			}
			//			0.19sec/60degrees move speed on servos
			if(ticks%15==0){
				xAngle = visionTable->GetNumber("xAngle");
				double moveAngle = (xAngle-26.4)*.5;
				side->SetAngle(side->GetAngle()+moveAngle);
				xTime = fabs((50/ 60)*moveAngle);

				ticks=0;
			}
			if(yticks%25==0){
				yAngle = visionTable->GetNumber("yAngle");
				double moveAngle = (19.8-yAngle);
				up->SetAngle(up->GetAngle()+moveAngle);
				yTime = fabs((50/60)*moveAngle);

				yticks=0;
			}
			if(up->GetAngle()>90){
				up->SetAngle(90);
			}
			ticks++;
			yticks++;
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

	}

	void TestInit() {
		std::cout<<"P: "<<shooter->getP()<<" I: "<<shooter->getI()<<" D: "<<shooter->getD()<<std::endl;
		//shooter->enablePID();
		//shooter->setMaxPower(.5);
		//test->SetClosedLoopControl(false);
		//shooter->tilt(RSHOOT_ANGLE);

	}

	//double setpointz = 0.17475;`

	void TestPeriodic()
	{

		//		if(intakeCounter % 60 == 0) {
		//			std::cout<<"Side: "<<side->GetAngle()<<" Tilt: "<<up->GetAngle()<<std::endl;;
		//
		////			std::cout << shooter->getAngle() << std::endl;
		////
		//		}
		//		intakeCounter++;

		////		lw->Run();


		//		drive->driveTank(lJoy->GetY(),rJoy->GetY());
		if(counter%60==0){
			std::cout<<"Pot: "<<shooter->getAngle()<<std::endl;
			counter=0;
		}
		//		counter++;
		//		if(controller->getY()){
		//			shooter->tilt(setpointz+.0001);
		//			setpointz = shooter->getSetpoint();
		//		}
		//		if(controller->getA()){
		//			shooter->tilt(setpointz -.0001);
		//			setpointz = shooter->getSetpoint();
		//		}
		//		if(controller->getB()) {
		//			shooter->tilt(RINTAKE_ANGLE);
		//		}
		//		if(controller->getStart()) {
		//			shooter->tilt(setpointz);
		//		}
		//		//if(controller->getButtonPress(Xbox::A))
		//		if(controller->getX()) {
		//			shooter->mShoot(.608);
		//			Wait(2);
		//			shooter->shootKicker(true);
		//			Wait(.5);
		//		}
		//		else {
		//			shooter->shootKicker(false);
		//			shooter->mShoot(0);
		//		}
		//shooter->mShoot(controller->getLX()*0.608);

		Wait(.05);
		//TestPID();
	}

	void TestPID() {
		if(controller->getA()) {
			shooter->setPID(shooter->getP() - .01, shooter->getI(), shooter->getD());
		}
		if(controller->getY()) {
			shooter->setPID(shooter->getP() + .01, shooter->getI(), shooter->getD());
		}
		if(controller->getX()) {
			shooter->setPID(shooter->getP(), shooter->getI() - .01, shooter->getD());
		}
		if(controller->getB()) {
			shooter->setPID(shooter->getP(), shooter->getI() + .01, shooter->getD());
		}
		if(controller->getBack()) {
			shooter->setPID(shooter->getP(), shooter->getI(), shooter->getD() - 0.01);
		}
		if(controller->getStart()) {
			shooter->setPID(shooter->getP(), shooter->getI(), shooter->getD()+ 0.01);
		}

		if(controller->getLB()) {
			shooter->tilt(.1755);
		}

		if(controller->getRB()) {
			shooter->tilt(RINTAKE_ANGLE);
		}

		if(counter % 60 == 0) {
			std::cout<<"P: "<<shooter->getP()<<" I: "<<shooter->getI()<<" D: "<<shooter->getD()<<std::endl;
			std::cout<<"Pot: "<<shooter->getAngle()<<" SetPoint: "<<shooter->getSetpoint()<<std::endl;
		}
	}

	void TestShooterPID() {
			if(controller->getA()) {
				shooter->setLeftShooterPID(shooter->getP() - .01, shooter->getI(), shooter->getD());
			}
			if(controller->getY()) {
				shooter->setLeftShooterPID(shooter->getP() + .01, shooter->getI(), shooter->getD());
			}
			if(controller->getX()) {
				shooter->setLeftShooterPID(shooter->getP(), shooter->getI() - .01, shooter->getD());
			}
			if(controller->getB()) {
				shooter->setLeftShooterPID(shooter->getP(), shooter->getI() + .01, shooter->getD());
			}
			if(controller->getBack()) {
				shooter->setLeftShooterPID(shooter->getP(), shooter->getI(), shooter->getD() - 0.01);
			}
			if(controller->getStart()) {
				shooter->setLeftShooterPID(shooter->getP(), shooter->getI(), shooter->getD()+ 0.01);
			}

			if(controller->getLB()) {

			}

			if(controller->getRB()) {
				shooter->tilt(RINTAKE_ANGLE);
			}

			if(counter % 60 == 0) {
				std::cout<<"P: "<<shooter->getShooterP()<<" I: "<<shooter->getShooterI()<<" D: "<<shooter->getShooterD()<<std::endl;
				std::cout<<"Speed: "<<shooter->getSpeed()<<" SetPoint: "<<shooter->getShooterSetpoint()<<std::endl;
			}
		}


	//.1688 , .637
};

START_ROBOT_CLASS(Robot)
