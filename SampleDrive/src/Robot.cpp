#include "WPIlib.h"
#include <math.h>
#include "Xbox.h"
#include "modules/RobotModule.h"
#include "modules/DriveModule.h"
#include "Peripherals/AutonomousCode/LogisticFunction.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include "WPILib.h"
#include "modules/ShooterModule.h"
#include "modules/DriveModule.h"
#include "Settings.h"
#include "modules/FlipperModule.h"
#include "modules/IntakeModule.h"
#include "Xbox.h"
//#include "CameraInput.h"

class Robot: public IterativeRobot
{
public:
	std::shared_ptr<NetworkTable> visionTable;
private:
	//offset = 41
	double movementInX=0;
	DriverStation::Alliance color;
	double xTime;
	double yTime;
	I2C *i2c;
	uint8_t lightPattern[1];
	uint8_t arduinoData[1];
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
	double setpointz = 0.34;
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
	bool cameras=false;
	int cameracount=0;
	int counter=0;
	double a = 3.897257 * pow(10, -6);
	double b = -0.0011549369;
	double c = 0.1656727552;
	double width = 0;
	AnalogInput* ultra;
	CameraInput* panInput;
	PIDController* panController;
	bool ball = false;
	bool detected = false;
	int counr = 0;

	bool real = true;

	void RobotInit()
	{
		visionTable = NetworkTable::GetTable("visionTable");
		panInput = new CameraInput(visionTable);
		test = new Compressor(0);
		up = new Servo(0);
		side = new Servo(1);
		lightPattern[0] = 0;
		i2c = new I2C(I2C::Port::kOnboard, 84);

		rJoy = new Joystick(1);
		lJoy = new Joystick(0);
		controller = new Xbox(2);
		intake = new IntakeModule(INTAKE_MOTOR, INTAKE_MOTOR_2, INTAKE_SOL);
		//intakeSolenoid = new Solenoid(0);
		drive = new DriveModule(DRIVE_LEFT1, DRIVE_LEFT2, DRIVE_RIGHT1, DRIVE_RIGHT2, DRIVE_ENCODER_1_A, DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B, (PIDSource*)panInput);
		shooter = new ShooterModule(POT, ANGLEMOTOR, SHOOTERMOTOR1, SHOOTERMOTOR2, SHOOTER_SOL, 9);
		shooter->createThread();
		tomahawks = new FlipperModule(TOMOHAWKS);
		//pcm = new PCM();
		ultra = new AnalogInput(3);

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
		//		drive->EnablePID(true);
		drive->drive(67);
		drive->turn(41);
		std::cout << "done" << std::endl;
	}

	// NOT BEING USED
	//	void autonGo(double distance, double time) {//Time in seconds for now
	//			LogisticFunction* func = new LogisticFunction(time, distance);
	//			std::cout << "Starting timer" << std::endl;
	//			Timer timer;
	//			timer.Start();
	//			drive->EnablePID(true);
	//			std::cout << "Starting function finding" << std::endl;
	//			while (timer.Get() < time) { //&& func->getDistance(timer.Get()-5 < distance) {
	//				drive->setDriveSetpoint(func->getDistance(timer.Get()));
	//				drive->setLeftPower(-drive->getDriveOutput());
	//				drive->setRightPower(drive->getDriveOutput());
	//				std::cout << drive->getDriveOutput() << std::endl;
	//			}
	//	//		std::cout << func->getA() << std::endl;
	//	//		std::cout << func->getDistance(2) << std::endl;
	//			drive->setLeftPower(0);
	//			drive->setRightPower(0);
	//			std::cout << "Finishing functions" << std::endl;
	//	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

		//		//		std::cout<<"yoyo"<<std::endl;
		//				frameWidth = visionTable->GetNumber("width");
		//				frameHeight = visionTable->GetNumber("height");
		//				Xtolerance = visionTable->GetNumber("XTolerance");
		//				Ytolerance = visionTable->GetNumber("YTolerance");
		//				yMovePerTick = visionTable->GetNumber("yTicks");
		//				xMovePerTick = visionTable->GetNumber("xTicks");

		//		lightPattern[0]=lightPattern[0]+1;
		std::cout<<"yoyoyo"<<std::endl;
		up->SetAngle(180);
		side->SetAngle(90);
		arcade = false;
		std::cout<<"yoyoyo"<<std::endl;
		shooter->enablePID();
		shooter->setMaxPower(.5);
		shooter->tilt(RSHOOT_ANGLE);
		std::cout<<"yoyoyo"<<std::endl;
		color = DriverStation::GetInstance().GetAlliance();
		lightPattern[0]=color+1;
		width = visionTable->GetNumber("width");
		//std::cout << "Autonomous starting" << std::endl;
		//autonGo(900,2);
	}

	void TeleopPeriodic()
	{
		detected = visionTable->GetNumber("detected");
		color = DriverStation::GetInstance().GetAlliance();
		//std::cout<<"color: "<<color+1<<std::endl;
		i2c->Write(84,lightPattern[0]);



		if (arcade) {
			drive->driveArcade(rJoy->GetY(),lJoy->GetX());
		} else {
			drive->driveTank(lJoy->GetY(), rJoy->GetY());
		}

		//std::cout << "Autonomous starting" << std::endl;


		intake->setSpinPower(controller->getLX());
		//if(rJoy->GetTrigger() && lJoy->GetTrigger() && !shooter->getShot()) {
		//shooter->shoot(1, 1, 2);
		//}
		//std::cout<<"yoyoyo"<<std::endl;

		if(lJoy->GetRawButton(3)) {
			drive->enablePan(true);
			drive->setPanSetpoint(width/2);
			if(!detected) {
				lightPattern[0]=4;
			}
			while(abs(drive->getPanInput() - drive->getPanSetpoint()) > 40) {
				drive->driveTank(drive->getPanOutput(), -drive->getPanOutput());
				detected = visionTable->GetNumber("detected");
				if(lJoy->GetRawButton(4)) {
					break;
				}
			}
			drive->driveTank(0,0);
			drive->enablePan(false);
		}
		if(controller->getRT()) {
			drive->driveTank(0,0);
			shooter->mShoot(1);
			Wait(1.5);
			shooter->shootKicker(true);
			Wait(.5);
		}
		else {
			shooter->shootKicker(false);
		}
		shooter->mShoot(-controller->getLX());
		//shooter->setAngleMotorPower(.2 * controller->getRX());
		//std::cout << "Autonomous starting" << std::endl;

		if(lJoy->GetRawButton(5)) {
			distance = visionTable->GetNumber("distance");
			distance = distance - 41;
			if(distance > DISTANCE4 && distance < DISTANCE5 && !as) {
				shooter->tilt(RSHOOT);
			}
			if(distance > DISTANCE3 && distance < DISTANCE4 && !as) {
				shooter->tilt(RSHOOT1);
			}
			if(distance > DISTANCE2 && distance < DISTANCE1 && !as) {
				shooter->tilt(RSHOOT2);

			}
		}
		if(lJoy->GetTrigger() && rJoy->GetTrigger()) {
			shooter->mShoot(RAMPOWER);
			Wait(2.5);
			shooter->shootKicker(true);
			Wait(.5);
		}
		if(controller->getLB() != intakes){
			intakeCounter++;
		}
		intakes = controller->getLB();
		if(lJoy->GetRawButton(2) != cameras){
			cameracount++;
		}
		cameras = lJoy->GetRawButton(2);
		//std::cout << "Autonomous starting" << std::endl;

		if(controller->getRB() != tomah){
			tomahawkCounter++;
		}
		if(controller->getA()){
			//if(intake->getStatus() || shooter->getSetpoint() == LEVEL_ANGLE)
			shooter->tilt(RINTAKE_ANGLE);
		}
		if(controller->getY()){
			//if(intake->getStatus || shooter->getSetpoint() >= LEVEL_ANGLE)
			shooter->tilt(RSHOOT);
		}
		if(controller->getStart()){
			//if(intake->getStatus || shooter->getSetpoint() >= LEVEL_ANGLE)
			shooter->tilt(RSHOOT_ANGLE);
		}
		if(controller->getX()) {
			//if(intake->getStatus || shooter->getSetpoint() == RINTAKE_ANGLE)
			shooter->tilt(LEVEL_ANGLE);
		}
		//		if(controller->getRB()) {
		//			shooter->mShoot(.608);
		//			Wait(2);
		//			shooter->shootKicker(true);
		//			Wait(.5);
		//			shooter->mShoot(0);
		//		}
		//		if(rJoy->GetRawButton(6)) {
		//			tomahawkCounter = 2;
		//			intakeCounter = 2;
		//		}
		if(!shooter->isBallIn()) {
			controller->SetRumble(Xbox::kRightRumble, .5);
			if(counr < 10) {
				lightPattern[0]=3;
			}
			else {
				lightPattern[0]=color + 1;

			}
			counr++;
			ball = true;

		}  else {
			ball =false;
			controller->SetRumble(Xbox::kRightRumble, 0);
			counr = 0;
		}
		tomah = controller->getRB();

		//std::cout << "Autonomous starting" << std::endl;

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
		//std::cout << "Autonomous starting" << std::endl;

		if(intakeCounter % 4 == 0){
			intake->retractIntake();
		}else if(intakeCounter % 2 == 0){
			intake->deployIntake();
		}
		if(cameracount % 4 == 0){
			up->SetAngle(180);
		}else if(cameracount % 2 == 0){
			up->SetAngle(150);
		}

		if(lJoy->GetRawButton(2)){
			up->Set(150);
		}
		//std::cout << "Autonomous starting" << std::endl;

		//		if(intakeCounter % 20 == 0) {
		//			std::cout << "controller value" << controller->getA() << std::endl;
		//			//std::cout << "value of solenoid" << intakeSolenoid->Get() << std::endl;
		//
		//		}
		//		intakeCounter++;
		//				std::cout<<"Yo"<<std::endl;
		//				Xtolerance = visionTable->GetNumber("XTolerance");
		//				Ytolerance = visionTable->GetNumber("YTolerance");
		//
		//				movementInX=0;
		//				movementInY=0;
		//				std::cout<<visionTable->GetNumber("detectedObject")<<std::endl;
		//				if(visionTable->GetNumber("detectedObject")==1){
		//					std::cout<<"ticks: "<<ticks<<std::endl;
		//
		//					x = visionTable->GetNumber("X");
		//					y = visionTable->GetNumber("Y");
		//
		//					if(x>(frameWidth/2+Xtolerance)){
		//						movementInX = -xMovePerTick;
		//					}
		//					if(x<(frameWidth/2-Xtolerance)){
		//						movementInX = xMovePerTick;
		//					}
		//					if(y>(frameHeight/2+Ytolerance)){
		//						movementInY=yMovePerTick;
		//					}
		//					else if(y<(frameHeight/2-Ytolerance)){
		//						movementInY=-yMovePerTick;
		//					}
		//					//			0.19sec/60degrees move speed on servos
		//					if(ticks%15==0){
		//						xAngle = visionTable->GetNumber("xAngle");
		//						double moveAngle = (xAngle-26.4)*.5;
		//						side->SetAngle(side->GetAngle()+moveAngle);
		//						xTime = fabs((50/ 60)*moveAngle);
		//
		//						ticks=0;
		//					}
		//					if(yticks%25==0){
		//						yAngle = visionTable->GetNumber("yAngle");
		//						double moveAngle = (19.8-yAngle);
		//						up->SetAngle(up->GetAngle()+moveAngle);
		//						yTime = fabs((50/60)*moveAngle);
		//
		//						yticks=0;
		//					}
		//					if(up->GetAngle()>90){
		//						up->SetAngle(90);
		//					}
		//					ticks++;
		//					yticks++;
		//				}
		//				else{
		//
		//					std::cout<<"Sweeping"<<std::endl;
		//					if(side->GetAngle()==180){
		//						movementFactor=-1;
		//					}
		//					if(side->GetAngle()==0){
		//						movementFactor=1;
		//					}
		//					std::cout<<side->GetAngle()<<std::endl;
		//
		//					side->SetAngle(side->GetAngle()+xMovePerTick*movementFactor*2);
		//				}

	}
	void DisabledPeriodic(){
		lightPattern[0] = 0; // Probably better to define enums for various light modes, but set a light mode here
		i2c->Write(84,lightPattern[0]);
		controller->SetRumble(Xbox::kRightRumble, 0);
	}

	void TestInit() {
		//std::cout<<"P: "<<shooter->getP()<<" I: "<<shooter->getI()<<" D: "<<shooter->getD()<<std::endl;
		//width = visionTable->GetNumber("width");
		shooter->enablePID();
		shooter->setMaxPower(.5);
		width = visionTable->GetNumber("width");
		//test->SetClosedLoopControl(false);
		shooter->tilt(RSHOOT_ANGLE);
		//shooter->tilt(RAMPOTPOSITION);
		//drive->EnablePID(true);
		//drive->enablePan(true);
	}
	double distance = 0;
	bool as = false;

	void TestPeriodic()
	{
		//TestForultra();
		distance = visionTable->GetNumber("distance");
		distance = distance - 41;
		shooter->mShoot(-controller->getLX());
		if(counter % 60 == 0) {
			std::cout<<"tilt:" << distance<< " setpoint: " << shooter->getSetpoint()<< std::endl;;

			//			std::cout << shooter->getAngle() << std::endl;
			//
		}
		counter++;

		////		lw->Run();
		if(controller->getBack()){
			intake->deployIntake();
		}
		else {
			intake->retractIntake();
		}

		if(controller->getRB()) {
			shooter->mShoot(1);
			Wait(2);
			shooter->shootKicker(true);
			Wait(.5);
		}

		drive->driveTank(lJoy->GetY(),rJoy->GetY());
		counter++;
		if(controller->getY()){
			shooter->tilt(RSHOOT);
		}
		if(controller->getA()){
			shooter->tilt(RSHOOT1);
		}
		if(controller->getB()) {
			shooter->tilt(RSHOOT2);
		}
		if(controller->getStart()) {
			shooter->tilt(RINTAKE_ANGLE);
			as = true;
		}
		else {
			as = false;
		}

		if(distance > DISTANCE4 && distance < DISTANCE5 && !as) {
			shooter->tilt(RSHOOT);
		}
		if(distance > DISTANCE3 && distance < DISTANCE4 && !as) {
			shooter->tilt(RSHOOT1);
		}
		if(distance > DISTANCE2 && distance < DISTANCE1 && !as) {
			shooter->tilt(RSHOOT2);
		}
		//if(controller->getButtonPress(Xbox::A))
		if(controller->getX()) {
			drive->driveTank(0,0);
			shooter->mShoot(1);
			Wait(2);
			shooter->shootKicker(true);
			Wait(.5);
		}
		else {
			shooter->shootKicker(false);
			shooter->mShoot(0);
		}
		shooter->mShoot(controller->getLX()*0.608);

		Wait(.05);
		if(lJoy->GetRawButton(3)) {
			drive->enablePan(true);
			drive->setPanSetpoint(width/2);
			if(!detected) {
				lightPattern[0]=4;
			}
			while(abs(drive->getPanInput() - drive->getPanSetpoint()) > 40) {
				drive->driveTank(drive->getPanOutput(), -drive->getPanOutput());
				detected = visionTable->GetNumber("detected");
				if(lJoy->GetRawButton(4)) {
					break;
				}
			}
			drive->driveTank(0,0);
			drive->enablePan(false);
		}
		//TestDrive();
		//TestPID();
		//TestShooterPID();
		//TestPan();
		Wait(.05);
	}

	void TestPID() {
		if(controller->getA()) {
			shooter->setPID(shooter->getP() - 0.00001, shooter->getI(), shooter->getD());
		}
		if(controller->getY()) {
			shooter->setPID(shooter->getP() + 0.00001, shooter->getI(), shooter->getD());
			std::cout << "increase P" << std::endl;
		}
		if(controller->getX()) {
			shooter->setPID(shooter->getP(), shooter->getI() - 0.01, shooter->getD());
		}
		if(controller->getB()) {
			shooter->setPID(shooter->getP(), shooter->getI() + 0.01, shooter->getD());
		}
		if(controller->getBack()) {
			shooter->setPID(shooter->getP(), shooter->getI(), shooter->getD() - 0.01);
		}
		if(controller->getStart()) {
			shooter->setPID(shooter->getP(), shooter->getI(), shooter->getD()+ 0.01);
		}

		if(controller->getLB()) {
			shooter->tilt(RAMPOTPOSITION);
		}

		if(controller->getRB()) {
			shooter->tilt(RINTAKE_ANGLE);
		}
		if(lJoy->GetTrigger() && rJoy->GetTrigger()) {
			shooter->mShoot(RAMPOWER);
			Wait(2.5);
			shooter->shootKicker(true);
			Wait(.5);
		}

		if(counter % 60 == 0) {
			std::cout<<"P: "<<shooter->getP()<<" I: "<<shooter->getI()<<" D: "<<shooter->getD()<<std::endl;
			std::cout<<"Pot: "<<shooter->getAngle()<<" SetPoint: "<<shooter->getSetpoint()<<std::endl;
		}
		counter++;
	}

	void TestShooterPID() {
		if(controller->getA()) {
			shooter->setLeftShooterPID(shooter->getShooterP() - 0.01, shooter->getShooterI(), shooter->getShooterD());
		}
		if(controller->getY()) {
			shooter->setLeftShooterPID(shooter->getShooterP() + 0.01, shooter->getShooterI(), shooter->getShooterD());
			std::cout << "increase P" << std::endl;
		}
		if(controller->getX()) {
			shooter->setLeftShooterPID(shooter->getShooterP(), shooter->getShooterI() - 0.01, shooter->getShooterD());
		}
		if(controller->getB()) {
			shooter->setLeftShooterPID(shooter->getShooterP(), shooter->getShooterI() + 0.01, shooter->getShooterD());
		}
		if(controller->getBack()) {
			shooter->setLeftShooterPID(shooter->getShooterP(), shooter->getShooterI(), shooter->getShooterD() - 0.01);
		}
		if(controller->getStart()) {
			shooter->setLeftShooterPID(shooter->getShooterP(), shooter->getShooterI(), shooter->getShooterD()+ 0.01);
		}

		if(controller->getLB()) {
			shooter->mShoot(1);
		}
		else {
			shooter->mShoot(0);
		}

		if(counter % 60 == 0) {
			std::cout<<"P: "<<shooter->getShooterP()<<" I: "<<shooter->getShooterI()<<" D: "<<shooter->getShooterD()<<std::endl;
			std::cout<<"Speed: "<<shooter->getSpeed()<< std::endl;
		}
		counter++;
	}

	void TestForultra() {
		drive->driveTank(lJoy->GetY(), rJoy->GetY());
		double x = ultra->GetVoltage()/.0049 *.393701 - 3.5;
		if(counter % 60 == 0) {
			std::cout << "shooter setpoint : " <<shooter->getSetpoint() << " ultra: "  << x*2.53 <<std::endl;
		}
		if(controller->getX()) {
			shooter->mShoot(1);
			Wait(2);
			shooter->shootKicker(true);
			Wait(.5);
		}
		else {
			shooter->shootKicker(false);
			shooter->mShoot(0);
		}
		counter++;

	}
	void TestDrive()  {

		//if(controller->getB()) {
		//drive->driveTank(0, .25);
		//}
		//else {
		//drive->driveTank(0,0);
		//}
		if(controller->getY()) {
			drive->setPID(drive->getP() + .001, drive->getI(), drive->getD());
		}
		if(controller->getA()) {
			drive->setPID(drive->getP() - .001, drive->getI(), drive->getD());
		}
		if(controller->getX()) {
			drive->setAngleSetpoint(3.5);
		}
		else {
			drive->setAngleSetpoint(0);
		}
		if(counter % 60 == 0) {
			std::cout <<" p : "  << drive->getP() <<" position: " <<drive->getLeftEncoder()- drive->getRightEncoder()  << "setpoint " << drive->getAngleSetpoint() << std::endl;
		}
		drive->driveTank(lJoy->GetY(), rJoy->GetY());
	}
	void TestPan() {
		up->SetAngle(150);
		if(controller->getY()) {
			drive->setPanPID(drive->getPanP() + .001, drive->getPanI(), drive->getPanD());
		}
		if(controller->getA()) {
			drive->setPanPID(drive->getPanP() - .001, drive->getPanI(), drive->getPanD());
		}
		if(controller->getX()) {
			drive->setPanPID(drive->getPanP(), drive->getPanI() - .01, drive->getPanD());
		}
		if(controller->getB()) {
			drive->setPanPID(drive->getPanP() , drive->getPanI() + .01, drive->getPanD());
		}
		if(controller->getBack()) {
			drive->setPanPID(drive->getPanP() , drive->getPanI(), drive->getPanD() - .01);
		}
		if(controller->getStart()) {
			drive->setPanPID(drive->getPanP() , drive->getPanI(), drive->getPanD() + .01);
		}
		if(controller->getRB())
			drive->setPanSetpoint(width/2);
		if(controller->getLB()) {
			drive->setPanSetpoint(0);
		}
		if(counter % 60 == 0) {
			std::cout <<" p : "  << drive->getPanP() << " i : " << drive->getPanI() << " d: " << drive->getPanD() << "setpoint " << drive->getPanSetpoint()  << " actual point: " << drive->getPanInput() << std::endl;
		}
		counter++;
		if(abs(drive->getPanInput() - drive->getPanSetpoint()) > 40)
			drive->driveTank(drive->getPanOutput(), -drive->getPanOutput());
		else {
			drive->driveTank(0,0);
		}
	}

	//.1688 , .637
};

START_ROBOT_CLASS(Robot)
