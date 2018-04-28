#include "WPILib.h"
#include <cmath>
#include "Xbox.h"
#include "modules/RobotModule.h"
#include "modules/DriveModule.h"
#include "../Peripherals/AutonomousCode/LogisticFunction.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <LiveWindow/LiveWindow.h>
#include "WPILib.h"
#include "modules/ShooterModule.h"
#include "modules/DriveModule.h"
#include "Settings.h"
#include "modules/FlipperModule.h"
#include "AHRS.h"
#include "modules/IntakeModule.h"
#include <networktables/NetworkTableInstance.h>

class Robot : public IterativeRobot {
public:
    std::shared_ptr<NetworkTable> visionTable;
private:
    //offset = 41
    DriverStation::Alliance color;

    I2C *i2c;
    uint8_t lightPattern[1];
    Compressor *test;
    bool down = false;
    double x;
    double y;
    LiveWindow *lw = LiveWindow::GetInstance();
    Servo *up;
    Servo *side;
    AHRS *navX; //navX
    bool arcade;

    IntakeModule *intake;
    DriveModule *drive;
    ShooterModule *shooter;
    Joystick *rJoy;
    Joystick *lJoy;
    int intakeCounter = 0;
    Xbox *controller;
    bool intakes = true;
    FlipperModule *tomahawks;
    bool cameras = false;
    int cameracount = 0;
    int counter = 0;
    double a = 3.897257 * pow(10, -6);
    double b = -0.0011549369;
    double c = 0.1656727552;
    double width = 0;
    AnalogInput *ultra;
    CameraInput *panInput;
    bool detected = false;
    int counr = 0;
    double testAngle = .79;
    bool real = true;

    void RobotInit() override {
        navX = new AHRS(SPI::kMXP); //navX
        visionTable = nt::NetworkTableInstance::GetDefault().GetTable("visionTable");
        panInput = new CameraInput(visionTable);
        test = new Compressor(0);
        up = new Servo(0);
        side = new Servo(1);
        lightPattern[0] = 0;
        i2c = new I2C(I2C::Port::kOnboard, 84);

        rJoy = new Joystick(1);
        lJoy = new Joystick(0);
        controller = new Xbox(4);
        intake = new IntakeModule(INTAKE_MOTOR_FORWARD, INTAKE_MOTOR_SIDEWAYS, INTAKE_SOL);
        drive = new DriveModule(DRIVE_LEFT1, DRIVE_LEFT2, DRIVE_RIGHT1, DRIVE_RIGHT2, DRIVE_ENCODER_1_A,
                                DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B, panInput, navX);
        shooter = new ShooterModule(POT, ANGLEMOTOR, SHOOTERMOTOR1, SHOOTERMOTOR2, SHOOTER_SOL, 9);
        shooter->createThread();
        tomahawks = new FlipperModule(TOMOHAWKS);
        ultra = new AnalogInput(3);
        std::cout << "yoyoyo" << std::endl;
        drive->calibrate();

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
    void pan() {
        drive->enablePan(true);
        drive->reset();
        double angle = visionTable->GetNumber("xAngle", 0);
        angle = angle - 31;

        drive->resetEncoders();
        auto *time = new Timer();
        time->Start();
        drive->setPanSetpoint(/*40/47.55*/  angle);
        while (time->Get() < 2 && !lJoy->GetRawButton(4) && (((drive->getAngle() - drive->getPanSetpoint()) > 2) ||
                                                             ((drive->getAngle() - drive->getPanSetpoint()) < -2))) {
            drive->driveTank(-drive->getPanOutput(), drive->getPanOutput());
        }

        drive->enablePan(false);
        drive->driveTank(0, 0);
    }

    void AutoStatic() {
        drive->EnablePID(true);
        shooter->enablePID();
        shooter->tilt(LEVEL_ANGLE);
        Wait(1);
        drive->reset();
        drive->resetEncoders();
        navX->ZeroYaw();
        auto *timeout = new Timer();
        timeout->Reset();
        timeout->Start();
        drive->drive(-160);

    }

    void AutoPortColis() {
        drive->EnablePID(true);
        drive->reset();
        shooter->enablePID();
        intake->deployIntake();
        tomahawks->Deploy();
        shooter->tilt(LEVEL_ANGLE);
        Wait(1);
        drive->drive(-100);
    }

    void AutoSeeSaw() {
        shooter->enablePID();
        shooter->tilt(LEVEL_ANGLE);
        Wait(1);
        drive->resetEncoders();
        navX->ZeroYaw();
        drive->drive(-40);
        Wait(.5);
        tomahawks->Deploy();
        Wait(.5);
        drive->EnablePID(false);
        drive->resetEncoders();
        drive->EnablePID(true);

        drive->drive(-30);
        Wait(.5);
        tomahawks->Retract();
        Wait(.5);
        drive->EnablePID(false);
        drive->resetEncoders();
        drive->EnablePID(true);

        drive->drive(-72);
        drive->driveTank(0, 0);
        drive->EnablePID(false);
    }


    void AutonomousInit() override {
        AutoStatic();
    }

    void AutonomousPeriodic() override {

    }

    void TeleopInit() override {
        std::cout << "yoyoyo" << std::endl;
        up->SetAngle(150);
        side->SetAngle(90);
        arcade = false;
        std::cout << "yoyoyo" << std::endl;
        shooter->enablePID();
        shooter->setMaxPower(.8);
        shooter->tilt(LEVEL_ANGLE);
        std::cout << "yoyoyo" << std::endl;
        color = DriverStation::GetInstance().GetAlliance();
        lightPattern[0] = color + 1;
    }

    double shootvalue = RSHOOT_ANGLE;

    void TeleopPeriodic() override {
        color = DriverStation::GetInstance().GetAlliance();
        i2c->Write(84, lightPattern[0]);


        if (arcade) {
            drive->driveArcade(rJoy->GetY(), lJoy->GetX());
        } else {
            drive->driveTank(lJoy->GetY(), rJoy->GetY());
        }

        intake->setSpinPower(controller->getLX());
        if (rJoy->GetRawButton(3)) {
            down = true;
            shooter->tilt(LEVEL_ANGLE);
            intake->deployIntake();
        } else if (down) {
            down = false;
            intakeCounter = 4;
        }
        std::cout << "Down: " << down << std::endl;

        if (lJoy->GetRawButton(3)) {
            drive->reset();
            drive->enablePan(true);
            double angle = visionTable->GetNumber("xAngle", 0);
            drive->setMaxPower(-.75, .75);
            angle = angle - 31;


            if (!detected) {
                lightPattern[0] = 4;
            }
            drive->resetEncoders();
            auto *time = new Timer();
            time->Start();
            drive->setPanSetpoint(/*40/47.55*/ VISION_TO_GYRO * angle);
            while (time->Get() < 2 && !lJoy->GetRawButton(4) && (((drive->getAngle() - drive->getPanSetpoint()) > 2) ||
                                                                 ((drive->getAngle()) - drive->getPanSetpoint()) <
                                                                 -2)) {
                drive->driveTank(-drive->getPanOutput(), drive->getPanOutput());
            }
            drive->enablePan(false);
            drive->driveTank(0, 0);
        }
        double power = -controller->getLX();
        if (power > .1 || power < -.1) {
            shooter->mShoot(-controller->getLX());
        } else {
            shooter->mShoot(0);
        }

        if (lJoy->GetRawButton(5)) {
            distance = visionTable->GetNumber("distance", 0);
            if (distance >= DISTANCE2 && distance <= DISTANCE3) {
                shooter->tilt(SHOOT2);
                std::cout << "in zone 2" << std::endl;

            }
            if (distance <= DISTANCE1) {
                shooter->tilt(SHOOT1);
                std::cout << "in zone 1" << std::endl;
            }
        }
        if (controller->getR3()) {
            shootvalue = shootvalue + .001;
        }
        if (controller->getL3()) {
            shootvalue = shootvalue - .001;
        }

        bool shouldFire = (lJoy->GetTrigger() && rJoy->GetTrigger()) ||
                          (controller->getLT() > .75 && controller->getRT() > .75);

        if (shouldFire) {
            shooter->mShoot(RAMPOWER);
            Wait(2.5);
            shooter->shootKicker(true);
            Wait(.5);
        } else {
            shooter->shootKicker(false);
        }

        if (controller->getLB() != intakes) {
            intakeCounter++;
        }
        intakes = controller->getLB();
        if (lJoy->GetRawButton(2) && lJoy->GetRawButton(2) != cameras) {
            cameracount++;
            std::cout << "print" << std::endl;
        }
        cameras = lJoy->GetRawButton(2);
        if (cameracount % 2 == 0) {
            up->SetAngle(150);

        }
        if (cameracount % 2 == 1) {
            up->SetAngle(180);

        }

        if (controller->getA()) {
            shooter->tilt(PINTAKE_ANGLE);
        }
        if (controller->getY()) {
            shooter->tilt(SHOOT1);
        }
        if (controller->getStart()) {
            shooter->tilt(SHOOT2);
        }
        if (controller->getX() || rJoy->GetRawButton(3)) {
            shooter->tilt(LEVEL_ANGLE);
        }

        if (!shooter->isBallIn()) {
            controller->SetRumble(Xbox::kRightRumble, .5);
            if (counr < 10) {
                lightPattern[0] = 3;
            } else {
                lightPattern[0] = color + 1;

            }
            counr++;

        } else {
            controller->SetRumble(Xbox::kRightRumble, 0);
            counr = 0;
        }


        tomahawks->Retract();

        //std::cout << "Autonomous starting" << std::endl;

        if (intakeCounter % 4 == 0 && !down) {
            intake->retractIntake();
        } else if (intakeCounter % 2 == 0 && !down) {
            intake->deployIntake();
        }

        counter++;

    }

    void DisabledPeriodic() override {
        lightPattern[0] = 0; // Probably better to define enums for various light modes, but set a light mode here
        i2c->Write(84, lightPattern[0]);
        controller->SetRumble(Xbox::kRightRumble, 0);
    }

    void TestInit() override {
        shooter->enablePID();
        shooter->setMaxPower(.8);
        shooter->tilt(.56);
        drive->enablePan(true);
    }

    double distance = 0;
    bool as = false;

    void TestPeriodic() override {

    }

    void TestPID() {
        if (controller->getA()) {
            shooter->setPID(shooter->getP() - 0.01, shooter->getI(), shooter->getD());
        }
        if (controller->getY()) {
            shooter->setPID(shooter->getP() + 0.01, shooter->getI(), shooter->getD());
            std::cout << "increase P" << std::endl;
        }
        if (controller->getX()) {
            shooter->setPID(shooter->getP(), shooter->getI() - 0.01, shooter->getD());
        }
        if (controller->getB()) {
            shooter->setPID(shooter->getP(), shooter->getI() + 0.01, shooter->getD());
        }
        if (controller->getBack()) {
            shooter->setPID(shooter->getP(), shooter->getI(), shooter->getD() - 0.01);
        }
        if (controller->getStart()) {
            shooter->setPID(shooter->getP(), shooter->getI(), shooter->getD() + 0.01);
        }

        if (controller->getLB()) {
            shooter->tilt(.56);
        }

        if (controller->getRB()) {
            shooter->tilt(.80);
        }

        if (counter % 60 == 0) {
            std::cout << "P: " << shooter->getP() << " I: " << shooter->getI() << " D: " << shooter->getD()
                      << std::endl;
            std::cout << "Pot: " << shooter->getAngle() << " SetPoint: " << shooter->getSetpoint() << std::endl;
        }
        counter++;
    }

    void findSetpoints() {
        double power = -controller->getLX();
        if (power > .1 || power < -.1) {
            shooter->mShoot(-controller->getLX());
        } else {
            shooter->mShoot(0);
        }
        intake->setSpinPower(controller->getLX());

        drive->driveTank(lJoy->GetY(), rJoy->GetY());
        if (controller->getRB()) {
            shooter->tilt(RINTAKE_ANGLE);
        }
        if (controller->getLB()) {
            shooter->tilt(testAngle);
        }

        if (lJoy->GetRawButton(3)) {
            drive->reset();
            drive->enablePan(true);
            double angle = visionTable->GetNumber("xAngle", 0);
            drive->setMaxPower(-.75, .75);
            if (angle > 31) {
                angle = angle - 31;
            } else {
                angle = angle - 31;
            }

            if (!detected) {
                lightPattern[0] = 4;
            }
            drive->resetEncoders();
            Timer *time = new Timer();
            time->Start();
            drive->setPanSetpoint(/*40/47.55*/ VISION_TO_GYRO * angle);
            while (time->Get() < 2 && !lJoy->GetRawButton(4) && (((drive->getAngle() - drive->getPanSetpoint()) > 2) ||
                                                                 ((drive->getAngle() - drive->getPanSetpoint()) <
                                                                  -2))) {
                drive->driveTank(-drive->getPanOutput(), drive->getPanOutput());
            }

            drive->enablePan(false);
            drive->driveTank(0, 0);
        }
        if (counter % 60 == 0) {
            if (controller->getA()) {
                testAngle += .01;
            }
            if (controller->getY()) {
                testAngle -= .01;
            }
            distance = visionTable->GetNumber("distance", 0);

            std::cout << "distance: " << distance << std::endl;
            std::cout << "Pan: " << navX->GetYaw() << std::endl;
            std::cout << "currangle: " << testAngle << std::endl;

        }
        counter++;

    }

    void TestForultra() {
        drive->driveTank(lJoy->GetY(), rJoy->GetY());
        double x = ultra->GetVoltage() / .0049 * .393701 - 3.5;
        if (counter % 60 == 0) {
            std::cout << "shooter setpoint : " << shooter->getSetpoint() << " ultra: " << x * 2.53 << std::endl;
        }
        if (controller->getX()) {
            shooter->mShoot(1);
            Wait(2);
            shooter->shootKicker(true);
            Wait(.5);
        } else {
            shooter->shootKicker(false);
            shooter->mShoot(0);
        }
        counter++;

    }

    void TestDrive() {

        drive->EnablePID(true);
        if (controller->getLB()) {
            drive->resetEncoders();
        }
        if (controller->getA()) {
            drive->setDrivePID(drive->getP() - .001, drive->getI(), drive->getD());
        }
        if (controller->getY()) {
            drive->setDrivePID(drive->getP() + .001, drive->getI(), drive->getD());
        }
        if (controller->getX()) {
            drive->setDrivePID(drive->getP(), drive->getI(), drive->getD() + .001);
        }
        if (controller->getB()) {
            drive->setDrivePID(drive->getP(), drive->getI(), drive->getD() - .001);
        }
        if (controller->getRB()) {
            drive->driveWithoutAngle(-43);
        } else {
            drive->setDriveSetpoint(0);
        }
        if (counter % 60 == 0) {
            std::cout << "p: " << drive->getP() << " position: " << drive->getLeftEncoder() << " setpoint:"
                      << drive->getDriveSetpoint() << std::endl;
        }
        counter++;
        drive->driveTank(drive->getDriveOutput(), drive->getDriveOutput());

    }

    void TestDriveWithAngle() {

        drive->EnablePID(true);
        if (controller->getLB()) {
            drive->resetEncoders();
        }
        if (controller->getA()) {
            drive->setDrivePID(drive->getP() - .001, drive->getI(), drive->getD());
        }
        if (controller->getY()) {
            drive->setDrivePID(drive->getP() + .001, drive->getI(), drive->getD());
        }
        if (controller->getX()) {
            drive->setDrivePID(drive->getP(), drive->getI(), drive->getD() + .001);
        }
        if (controller->getB()) {
            drive->setDrivePID(drive->getP(), drive->getI(), drive->getD() - .001);
        }
        if (controller->getRB()) {

            drive->drive(-43);
        } else {
            drive->drive(0);
        }
        if (counter % 60 == 0) {
            std::cout << "p: " << drive->getP() << " position: " << drive->getLeftEncoder() << " setpoint:"
                      << drive->getDriveSetpoint() << std::endl;
        }
        counter++;
        drive->driveTank(drive->getDriveOutput(), drive->getDriveOutput());
    }

    void TestPan() {
        up->SetAngle(150);
        if (controller->getY()) {
            drive->setPanPID(drive->getPanP() + .0001, drive->getPanI(), drive->getPanD());
        }
        if (controller->getA()) {
            drive->setPanPID(drive->getPanP() - .0001, drive->getPanI(), drive->getPanD());
        }
        if (controller->getX()) {
            drive->setPanPID(drive->getPanP(), drive->getPanI() - .00001, drive->getPanD());
        }
        if (controller->getB()) {
            drive->setPanPID(drive->getPanP(), drive->getPanI() + .00001, drive->getPanD());
        }
        if (controller->getBack()) {
            drive->setPanPID(drive->getPanP(), drive->getPanI(), drive->getPanD() - .01);
        }
        if (controller->getStart()) {
            drive->setPanPID(drive->getPanP(), drive->getPanI(), drive->getPanD() + .01);
        }
        if (controller->getRB())
            drive->setPanSetpoint(0);
        if (controller->getLB()) {
            drive->setPanSetpoint(10);
        }
        if (controller->getRT()) {
            drive->setPanSetpoint(40);
        }
        if (controller->getLT()) {
            drive->setPanSetpoint(40);
        }
        if (counter % 60 == 0) {
            std::cout << " p : " << drive->getPanP() << " i : " << drive->getPanI() << " d: " << drive->getPanD()
                      << "setpoint " << drive->getPanSetpoint() << " actual point: " << drive->getAngle() << std::endl;
        }
        counter++;
        if (lJoy->GetTrigger()) {
            drive->driveTank(lJoy->GetY(), rJoy->GetY());
        }
        drive->driveTank(-drive->getPanOutput(), drive->getPanOutput());

    }

    void TestEverything() {
        shooter->tilt(LEVEL_ANGLE);
        auto *times = new Timer();
        times->Start();
        while (times->Get() < 4) {
            drive->driveTank(1, 1);
        }
        drive->driveTank(0, 0);
        shooter->tilt(RSHOOT);
        Wait(.5);
        up->SetAngle(150);
        Wait(.25);
    }

    //.1688 , .637
};

START_ROBOT_CLASS(Robot)
