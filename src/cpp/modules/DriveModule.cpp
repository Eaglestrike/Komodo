#include <modules/DriveModule.h>

DriveModule::DriveModule(int lTal1, int lTal2, int rTal1, int rTal2, int lEncA, int lEncB, int rEncA, int rEncB,
                         AHRS *gyro) : RobotModule() {
    rTalon1 = new WPI_TalonSRX(rTal1);
    rTalon2 = new WPI_TalonSRX(rTal2);
    lTalon1 = new WPI_TalonSRX(lTal1);
    lTalon2 = new WPI_TalonSRX(lTal2);

    rEnc = new Encoder(rEncA, rEncB);
    lEnc = new Encoder(lEncA, lEncB);
    panOut = new DriveOut();
    this->gyro = gyro;
    gyro->ZeroYaw();

    lEnc->SetReverseDirection(true);
    driveIn = new DriveIn(rEnc, lEnc);
    driveOut = new DriveOut();
    drive_controller = new PIDController(DRIVE_CONTROLLER_P, DRIVE_CONTROLLER_I, DRIVE_CONTROLLER_D, driveIn,
                                         driveOut);

    drive_controller->SetOutputRange(-.75, .75);
    angleIn = new AngleIn(gyro);
    angleOut = new DriveOut();

    angle_controller = new PIDController(ANGLE_CONTROLLER_P, ANGLE_CONTROLLER_I, ANGLE_CONTROLLER_D, angleIn,
                                         angleOut);
    angle_controller->SetOutputRange(-.5, .5);
    pan = new AngleIn(gyro);
    pan_controller = new PIDController(0.0441, 0, 0.13, pan, panOut);
    rEnc->Reset();
    lEnc->Reset();
}

void DriveModule::setRightPower(double rPow) {
    rTalon1->Set(ControlMode::PercentOutput, rPow);
    rTalon2->Set(ControlMode::PercentOutput, rPow);
}

void DriveModule::setLeftPower(double lPow) {
    lTalon1->Set(ControlMode::PercentOutput, lPow);
    lTalon2->Set(ControlMode::PercentOutput, lPow);
}

void DriveModule::enablePID(bool enable) {
    if (enable) {
        drive_controller->Enable();
        angle_controller->Enable();
    } else {
        drive_controller->Disable();
        angle_controller->Disable();
    }
}

void DriveModule::driveArcade(double throttle, double angle) {

    setLeftPower((throttle + angle) / -2);
    setRightPower((throttle - angle) / 2);

    //I am 99% sure that this wont work
//    leftMotorOutput = 0;
//    rightMotorOutput = 0;
//
//    if (throttle > 0.0) {
//        angle = -angle;
//        if (angle < 0.0) {
//            leftMotorOutput = throttle + angle;
//            rightMotorOutput = fmax(throttle, -angle);
//        } else {
//            leftMotorOutput = fmax(throttle, angle);
//            rightMotorOutput = throttle - angle;
//        }
//    } else {
//        if (angle > 0.0) {
//            leftMotorOutput = -fmax(-throttle, angle);
//            rightMotorOutput = throttle + angle;
//        } else {
//            leftMotorOutput = throttle - angle;
//            rightMotorOutput = -fmax(-throttle, -angle);
//        }
//    }
//    setLeftPower(-leftMotorOutput);
//    setRightPower(rightMotorOutput);
}

void DriveModule::driveTank(double lPow, double rPow) {
    setLeftPower(-lPow);
    setRightPower(rPow);
}

void DriveModule::setDriveSetpoint(double setpoint) {
    drive_controller->SetSetpoint(setpoint * TICKS_PER_INCH);
}

double DriveModule::getDriveSetpoint() {
    return drive_controller->GetSetpoint();
}

void DriveModule::setAngleSetpoint(double angle) {
    angle_controller->SetSetpoint(angle);
}

double DriveModule::getAngleSetpoint() {
    return angle_controller->GetSetpoint();
}

double DriveModule::getRightEncoder() {
    return rEnc->PIDGet();
}

double DriveModule::getLeftEncoder() {
    return lEnc->PIDGet();
}

void DriveModule::setMaxPower(double min, double max) {
    drive_controller->SetOutputRange(min, max);
    angle_controller->SetOutputRange(min, max);
}

void DriveModule::drive(double setpoint) {
    resetEncoders();
    Timer *time = new Timer();
    time->Start();
    enablePID(true);
    setDriveSetpoint(setpoint);
    setAngleSetpoint(0);
    while (time->Get() < 5 && abs(driveIn->PIDGet() - getDriveSetpoint()) > 2) {
        driveTank(-driveOut->getPower() - angleOut->getPower(), -driveOut->getPower() + angleOut->getPower());
    }
    enablePID(false);
    driveTank(0, 0);
}

void DriveModule::driveWithoutAngle(double setpoint) {
    resetEncoders();
    Timer *time = new Timer();
    time->Start();
    enablePID(true);
    setDriveSetpoint(setpoint);
    while (time->Get() < 5 && abs(driveIn->PIDGet() - getDriveSetpoint()) > 2) {
        std::cout << getRightEncoder() << std::endl;
        driveTank(-driveOut->getPower(), -driveOut->getPower());
    }
    enablePID(false);
    driveTank(0, 0);
}

double DriveModule::getAngle() {
    return gyro->GetYaw();
}

void DriveModule::turn(double angle) {
    resetEncoders();
    auto *time = new Timer();
    time->Start();
    enablePID(true);
    setAngleSetpoint(angle);
    while (time->Get() < 4 &&
           (((angleIn->PIDGet() - getAngleSetpoint()) > 1) || ((angleIn->PIDGet() - getAngleSetpoint()) < -1))) {
        driveTank(-angleOut->getPower(), angleOut->getPower());
    }
    enablePID(false);
    driveTank(0, 0);
}

void DriveModule::turnALPHA(double angle) {
    auto *time = new Timer();
    time->Start();
    enablePID(true);
    setAngleSetpoint(angle);
    while (time->Get() < 4 &&
           (((angleIn->PIDGet() - getAngleSetpoint()) > 1) || ((angleIn->PIDGet() - getAngleSetpoint()) < -1))) {
        driveTank(-angleOut->getPower(), angleOut->getPower());
    }
    resetEncoders();
    enablePID(false);
    driveTank(0, 0);
}

void DriveModule::setPanSetpoint(double setPoint) {
    pan_controller->SetSetpoint(setPoint);
}

void DriveModule::enablePan(bool enable) {
    pan_controller->SetEnabled(enable);
}

double DriveModule::getPanOutput() {
    return panOut->getPower();
}

double DriveModule::getPanSetpoint() {
    return pan_controller->GetSetpoint();
}

//Reseting the gyro constantly seems like a bad idea
void DriveModule::reset() {
    gyro->Reset();
}

void DriveModule::resetEncoders() {
    rEnc->Reset();
    lEnc->Reset();
}
