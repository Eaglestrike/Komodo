#include <modules/ShooterModule.h>

ShooterModule::ShooterModule(int anglePort, int angleMotorPort, int leftPort, int rightPort, int solenoidPort,
                             int buttonPort) : RobotModule() {
    angle = new AnalogPotentiometer(anglePort);
    angleMotor = new WPI_TalonSRX(angleMotorPort);

    leftShooter = new WPI_TalonSRX(leftPort);
    rightShooter = new WPI_TalonSRX(rightPort);

    shooterSol = new Solenoid(solenoidPort);

    button = new DigitalInput(buttonPort);

    leftShooter->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
    shootIn = new ShooterIn(angle);

    angleController = new PIDController(SHOOTER_CONTROLLER_P, SHOOTER_CONTROLLER_I, SHOOTER_CONTROLLER_D,
                                        shootIn, angleMotor);
    createThread();
}

ShooterModule::~ShooterModule() = default;

//This is un-used but may be helpful
double ShooterModule::getAngle() {
    return angle->Get();
}

bool ShooterModule::isBallIn() {
    return !button->Get();
}

void ShooterModule::createThread() {
    std::thread t(ShooterModule::callRun, this);
    t.detach();
}

void ShooterModule::callRun(void *m) {
    ((ShooterModule *) m)->run();
}

void ShooterModule::run() {
    shoot(1, 2.5);
}

//May be broken
void ShooterModule::shoot(double shootSpeed, double time) {
    setShooterSpeed(shootSpeed);
    Wait(time);
    shooterSol->Set(true);
    Wait(.5);
    setShooterSpeed(0);
    shooterSol->Set(false);
}

void ShooterModule::shootKicker(bool kick) {
    shooterSol->Set(kick);
}

void ShooterModule::setShooterAngle(double angle) {
    angleController->SetSetpoint(angle < MINIMUM_ANGLE ? MINIMUM_ANGLE : angle > MAXIMUM_ANGLE ? MAXIMUM_ANGLE : angle);
}

void ShooterModule::setShooterSpeed(double speed) {
    rightShooter->Set(ControlMode::PercentOutput, speed);
    leftShooter->Set(ControlMode::PercentOutput, -speed);
}

void ShooterModule::enablePID() {
    angleController->Enable();
}

void ShooterModule::setMaxPower(double power) {
    angleController->SetOutputRange(-power, power);
}
