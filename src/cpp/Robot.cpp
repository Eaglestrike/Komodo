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

    DriverStation::Alliance color;

    I2C *i2c;
    uint8_t lightPattern[1];

    //We define this but it is never used.
    //We may need to add limits to it so it won't just use its unknown default value.
    Compressor *compressor = new Compressor();

    bool isIntakeDown = false;

    //Like smart dashboard?
    LiveWindow *lw = LiveWindow::GetInstance();

    Servo *visionVerticalAngleServo;
    Servo *visionPivotAngleServo;


    AHRS *navX;
    bool arcade;

    IntakeModule *intake;
    DriveModule *drive;
    ShooterModule *shooter;
    FlipperModule *tomahawks;

    //Control systems
    Joystick *rJoy;
    Joystick *lJoy;
    Xbox *controller;

    int intakeCounter = 0;
    bool intakes = true;
    bool enableCameras = true;
    int cameracount = 0;
    int counter = 0;

    double a = 3.897257E-6;
    double b = -0.0011549369;
    double c = 0.1656727552;
    //double width = 0;
    //bool detected = false;
    int counr = 0;

    void RobotInit() override {
        navX = new AHRS(SPI::kMXP);
        
        //Do vision setup
        visionTable = nt::NetworkTableInstance::GetDefault().GetTable("visionTable");
        visionVerticalAngleServo = new Servo(0);
        visionPivotAngleServo = new Servo(1);

        //Setup communication to LED arduino
        lightPattern[0] = 0;
        i2c = new I2C(I2C::Port::kOnboard, 84);

        //Define controllers
        rJoy = new Joystick(1);
        lJoy = new Joystick(0);
        controller = new Xbox(4);

        //Define modules
        intake = new IntakeModule(INTAKE_MOTOR_FORWARD, INTAKE_MOTOR_SIDEWAYS, INTAKE_SOL);
        drive = new DriveModule(DRIVE_LEFT1, DRIVE_LEFT2, DRIVE_RIGHT1, DRIVE_RIGHT2, DRIVE_ENCODER_1_A,
                                DRIVE_ENCODER_1_B, DRIVE_ENCODER_2_A, DRIVE_ENCODER_2_B, navX);
        shooter = new ShooterModule(POT, ANGLEMOTOR, SHOOTERMOTOR1, SHOOTERMOTOR2, SHOOTER_SOL, 9);
        tomahawks = new FlipperModule(TOMOHAWKS);
    }

    void AutoStatic() {
        drive->enablePID(true);
        shooter->enablePID();
        shooter->setShooterAngle(LEVEL_ANGLE);
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
        visionVerticalAngleServo->SetAngle(150);
        visionPivotAngleServo->SetAngle(90);
        arcade = false;
        std::cout << "yoyoyo" << std::endl;
        shooter->enablePID();
        shooter->setMaxPower(.8);
        shooter->setShooterAngle(LEVEL_ANGLE);
        std::cout << "yoyoyo" << std::endl;
        color = DriverStation::GetInstance().GetAlliance();
        lightPattern[0] = color + 1;
    }

    //double shootvalue = SHOOT_ANGLE;

    void TeleopPeriodic() override {
        color = DriverStation::GetInstance().GetAlliance();

        //Set LED mode
        lightPattern[0] = static_cast<uint8_t>(isIntakeDown ? 1 : 0);
        i2c->Write(84, lightPattern[0]);

        if (controller->getBack()) {
            drive->driveTank(0, 0);
            intake->setSpinPower(0);
            shooter->setShooterSpeed(0);
            return;
        }

        //Drive mode
        //TODO: Add cheesy drive option
        if (!lJoy->GetRawButton(3)) {
            if (arcade) {
                drive->driveArcade(rJoy->GetY(), lJoy->GetX());
            } else {
                drive->driveTank(lJoy->GetY(), rJoy->GetY());
            }
        } else {
            drive->driveTank(0, 0);
        }

        //Do intake speed
        intake->setSpinPower(controller->getLX());

        //Put down intake
        if (rJoy->GetRawButton(3)) {
            isIntakeDown = true;
            //TODO: LEVEL_ANGLE is too high to pick up ball.
            shooter->setShooterAngle(LEVEL_ANGLE);
            intake->deployIntake();
        } else if (isIntakeDown) {
            isIntakeDown = false;
            intakeCounter = 4;
        }


        if (lJoy->GetRawButton(3)) {
            drive->reset();
            drive->enablePan(true);
            double angle = visionTable->GetNumber("xAngle", 0);
            drive->setMaxPower(-.75, .75);
            angle -= 31;
            drive->resetEncoders();
            auto *time = new Timer();
            time->Start();
            drive->setPanSetpoint(VISION_TO_GYRO * angle);
            while (time->Get() < 2 && !lJoy->GetRawButton(4) && (drive->getAngle() - drive->getPanSetpoint() > 2 ||
                                                                 drive->getAngle() - drive->getPanSetpoint() < -2)) {
                drive->driveTank(-drive->getPanOutput(), drive->getPanOutput());
            }
            drive->enablePan(false);
            drive->driveTank(0, 0);
        }

        shooter->setShooterSpeed(deadZone(-controller->getLX(), 0.1));

        //If the robot should fire
        if ((lJoy->GetTrigger() && rJoy->GetTrigger()) || (controller->getLT() > .75 && controller->getRT() > .75)) {
            shooter->run();
        } else {
            shooter->shootKicker(false);
        }

        //This will toggle twice on press then release
        //May be an issue
        if (controller->getLB() != intakes) {
            intakeCounter++;
        }
        intakes = controller->getLB();


        if (lJoy->GetRawButton(2) && enableCameras) {
            cameracount++;
            std::cout << "print" << std::endl;
        }
        enableCameras = lJoy->GetRawButton(2);

        visionVerticalAngleServo->SetAngle(cameracount % 2 == 0 ? 150 : 180);

        if (controller->getA()) {
            shooter->setShooterAngle(INTAKE_ANGLE);
        }
        if (controller->getY()) {
            shooter->setShooterAngle(SHOOT1);
        }
        if (controller->getStart()) {
            shooter->setShooterAngle(SHOOT2);
        }
        if (controller->getX() || rJoy->GetRawButton(3)) {
            shooter->setShooterAngle(LEVEL_ANGLE);
        }

        if (!shooter->isBallIn()) {
            controller->setRumble(RUMBLE);
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

        //TODO: This may not be needed, test if we can remove this with no effects
        if (intakeCounter % 4 == 0 && !isIntakeDown) {
            intake->retractIntake();
        } else if (intakeCounter % 2 == 0 && !isIntakeDown) {
            intake->deployIntake();
        }

        counter++;

    }

    double limit(double in, double min, double max) {
        return fmin(fmax(in, min), max);
    }

    double deadZone(double in, double zoneSize) {
        return abs(in) <= zoneSize ? 0 : in;
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
        shooter->setShooterAngle(.56);
        drive->enablePan(true);
    }

    void TestPeriodic() override {}

};


START_ROBOT_CLASS(Robot)
