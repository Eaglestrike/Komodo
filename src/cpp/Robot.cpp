#include "WPILib.h"
#include "Xbox.h"
#include "modules/DriveModule.h"
#include <LiveWindow/LiveWindow.h>
#include "modules/ShooterModule.h"
#include "modules/FlipperModule.h"
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
    AHRS *navX;
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
    bool detected = false;
    int counr = 0;
    bool real = true;

    void RobotInit() override {
        navX = new AHRS(SPI::kMXP);
        visionTable = nt::NetworkTableInstance::GetDefault().GetTable("visionTable");
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
                                DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B, navX);
        shooter = new ShooterModule(POT, ANGLEMOTOR, SHOOTERMOTOR1, SHOOTERMOTOR2, SHOOTER_SOL, 9);
        shooter->createThread();
        tomahawks = new FlipperModule(TOMOHAWKS);
        std::cout << "yoyoyo" << std::endl;
        drive->calibrate();

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


    void AutonomousInit() override {
        AutoStatic();
    }

    void AutonomousPeriodic() override {}

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

    double shootvalue = SHOOT_ANGLE;

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
            shooter->tilt(INTAKE_ANGLE);
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
            controller->setRRumble(.5);
            if (counr < 10) {
                lightPattern[0] = 3;
            } else {
                lightPattern[0] = color + 1;

            }
            counr++;

        } else {
            controller->setRumble(0);
            counr = 0;
        }


        tomahawks->Retract();
        if (intakeCounter % 4 == 0 && !down) {
            intake->retractIntake();
        } else if (intakeCounter % 2 == 0 && !down) {
            intake->deployIntake();
        }

        counter++;

    }

    void DisabledPeriodic() override {
        // Probably better to define enums for various light modes, but set a light mode here
        lightPattern[0] = 0;
        i2c->Write(84, lightPattern[0]);
        controller->setRumble(0);
    }

    void TestInit() override {
        shooter->enablePID();
        shooter->setMaxPower(.8);
        shooter->tilt(.56);
        drive->enablePan(true);
    }

    double distance = 0;
    bool as = false;

    void TestPeriodic() override {}

};


START_ROBOT_CLASS(Robot)