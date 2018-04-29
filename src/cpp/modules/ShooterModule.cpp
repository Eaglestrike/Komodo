#include <modules/ShooterModule.h>

ShooterModule::ShooterModule(int anglePort, int angleMotorPort, int leftport, int rightport, int solenoidPort,
                             int buttonport) : RobotModule() {
    // TODO Auto-generated constructor stub
    angle = new AnalogPotentiometer(anglePort);
    angleMotor = new WPI_TalonSRX(angleMotorPort);
    leftShooter = new WPI_TalonSRX(leftport);

    rightShooter = new WPI_TalonSRX(rightport);
    shooterSol = new Solenoid(solenoidPort);
    button = new DigitalInput(buttonport);
    leftShooter->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
    shootIn = new ShooterIn(angle);

    angleController = new PIDController(SHOOTER_CONTROLLER_P, SHOOTER_CONTROLLER_I, SHOOTER_CONTROLLER_D,
                                            shootIn, angleMotor);
}

ShooterModule::~ShooterModule() = default;

double ShooterModule::getAngle() {
    return angle->Get();
}

bool ShooterModule::isBallIn() {
    return !button->Get();
}

void ShooterModule::createThread() {
    std::thread t(ShooterModule::callrun, this);
    t.detach();
}

void ShooterModule::callrun(void *m) {
    ((ShooterModule *) m)->run();
}

void ShooterModule::run() {
    shoot(1, 1, 1);
}

bool ShooterModule::getShot() {
    return shot;
}

void ShooterModule::shoot(double left, double right, double time) {
    shot = true;
    shooterSol->Set(true);
    leftShooter->Set(ControlMode::PercentOutput, -left);
    rightShooter->Set(ControlMode::PercentOutput, right);
    Wait(time);
    leftShooter->Set(ControlMode::PercentOutput, 0);
    rightShooter->Set(ControlMode::PercentOutput, 0);
    shooterSol->Set(false);
    shot = false;
}

void ShooterModule::shootKicker(bool kick) {
    shooterSol->Set(kick);
}

void ShooterModule::mShoot(double power) {
    rightShooter->Set(ControlMode::PercentOutput, power);
    leftShooter->Set(ControlMode::PercentOutput, -power);

}

void ShooterModule::setAngleMotorPower(double power) {
    angleMotor->Set(ControlMode::PercentOutput, power);
}

void ShooterModule::tilt(double angle) {
    if (angle > MAXIMUM_ANGLE) {
        angle = MAXIMUM_ANGLE;
    }
    if (angle < MINIMUM_ANGLE) {
        angle = MINIMUM_ANGLE;
    }
    angleController->SetSetpoint(angle);
}

void ShooterModule::speed(double speed) {
    rightShooter->Set(ControlMode::PercentOutput, speed);
}

double ShooterModule::getSpeed() {
    return rightShooter->GetSelectedSensorVelocity(0);
}

void ShooterModule::setPID(double p, double i, double d) {
    angleController->SetPID(p, i, d);
}

void ShooterModule::setShooterSpeed(double speed) {
    rightShooter->Set(ControlMode::PercentOutput, speed);
}

double ShooterModule::getP() {
    return angleController->GetP();
}

double ShooterModule::getI() {
    return angleController->GetI();
}

double ShooterModule::getD() {
    return angleController->GetD();
}

void ShooterModule::enablePID() {
    angleController->Enable();
}

void ShooterModule::disablePID() {
    angleController->Disable();
}

void ShooterModule::setMaxPower(double power) {
    angleController->SetOutputRange(-power, power);
}

double ShooterModule::getSetpoint() {
    return angleController->GetSetpoint();
}

