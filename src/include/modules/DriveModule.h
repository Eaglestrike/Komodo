#ifndef SRC_MODULES_DRIVEMODULE_H_
#define SRC_MODULES_DRIVEMODULE_H_

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include "AHRS.h"
#include "Settings.h"
#include "CameraInput.h"
#include "RobotModule.h"

class DriveIn : public PIDSource {
private:
    Encoder *rEnc;
    Encoder *lEnc;

public:
    virtual ~DriveIn() = default;

    DriveIn(Encoder *rEncInput, Encoder *lEncInput) {
        rEnc = rEncInput;
        lEnc = lEncInput;
    }

    double PIDGet() override {
        return rEnc->PIDGet();
    }
};

class AngleIn : public PIDSource {
private:
    AHRS *gyro;

public:
    virtual ~AngleIn() = default;

    explicit AngleIn(AHRS *g) {
        gyro = g;
    }

    double PIDGet() override {
        return gyro->GetYaw();
    }
};

class DriveOut : public PIDOutput {

public:
    DriveOut() = default;

    virtual ~DriveOut() = default;

    void PIDWrite(double output) override {
        power = output;
    }

    double getPower() {
        return power;
    }

private:
    double power = 0;
};

class DriveModule : public RobotModule {
public:
    DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB,
                AHRS *gyro);

    void setRightPower(double rPow);

    void setLeftPower(double lPow);

    void driveArcade(double throttle, double angle);

    void driveTank(double lPow, double rPow);

    void setDriveSetpoint(double setpoint);

    double getDriveSetpoint();

    double getRightEncoder();

    double getLeftEncoder();

    void enablePID(bool enable);

    void setAngleSetpoint(double angle);

    double getAngleSetpoint();

    void drive(double setpoint);

    void driveWithoutAngle(double setpoint);

    void turn(double angle);

    void setPanSetpoint(double setPoint);

    void enablePan(bool enable);

    void turnALPHA(double angle);

    double getPanOutput();

    double getPanSetpoint();

    double getAngle();

    void setMaxPower(double min, double max);

    void reset();

    void resetEncoders();

private:
    WPI_TalonSRX *rTalon1;
    WPI_TalonSRX *rTalon2;
    WPI_TalonSRX *lTalon1;
    WPI_TalonSRX *lTalon2;

    Encoder *lEnc;
    Encoder *rEnc;
    DriveIn *driveIn;
    DriveOut *driveOut;
    AngleIn *angleIn;
    DriveOut *angleOut;
    DriveOut *panOut;
    PIDController *drive_controller;
    PIDController *angle_controller;
    PIDController *pan_controller;
    PIDSource *pan;
    AHRS *gyro;
};


#endif /* SRC_MODULES_DRIVEMODULE_H_ */
