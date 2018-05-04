#include "WPILib.h"
#include <ctre/Phoenix.h>
#include "Settings.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include "RobotModule.h"

#ifndef SRC_MODULES_SHOOTERMODULE_H_
#define SRC_MODULES_SHOOTERMODULE_H_


class ShooterIn : public PIDSource {
private:
    AnalogPotentiometer *p1;

public:
    virtual ~ShooterIn() = default;

    explicit ShooterIn(AnalogPotentiometer *a) {
        p1 = a;
    }

    double PIDGet() override {
        return p1->Get();
    }

};

class ShooterModule : public RobotModule {
public:
    ShooterModule(int anglePort, int angleMotorPort, int leftPort, int rightPort, int solenoidPort, int buttonPort);

    ~ShooterModule() override;

    void setShooterAngle(double angle);

    void shoot(double shootSpeed, double time);

    double getAngle();

    void shootKicker(bool kick);

    bool isBallIn();

    void setShooterSpeed(double speed);

    void run();

    void enablePID();

    void setMaxPower(double power);

    static void callRun(void *);

    void createThread();

private:
    AnalogPotentiometer *angle;
    WPI_TalonSRX *angleMotor;
    WPI_TalonSRX *rightShooter;
    WPI_TalonSRX *leftShooter;
    Solenoid *shooterSol;

    ShooterIn *shootIn;
    PIDController *angleController;
    DigitalInput *button;
};

#endif /* SRC_MODULES_SHOOTERMODULE_H_ */
