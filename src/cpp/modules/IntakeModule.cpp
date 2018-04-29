#include <modules/IntakeModule.h>

IntakeModule::IntakeModule(int spinInput, int spinInput2, int moveInput) : RobotModule() {
    spinTalon = new WPI_TalonSRX(spinInput);
    spinTalon2 = new WPI_TalonSRX(spinInput2);
    moveSol = new Solenoid(moveInput);

}

void IntakeModule::setSpinPower(double pow) {
    spinTalon->Set(ControlMode::PercentOutput, pow);
    spinTalon2->Set(ControlMode::PercentOutput, pow);
}

void IntakeModule::retractIntake() {
    moveSol->Set(false);
}

void IntakeModule::deployIntake() {
    moveSol->Set(true);
}

void IntakeModule::alternateIntake() {
    moveSol->Set(!moveSol->Get());
}

bool IntakeModule::getStatus() {
    return moveSol->Get();
}
