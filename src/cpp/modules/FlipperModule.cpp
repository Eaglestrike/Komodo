#include <modules/FlipperModule.h>

FlipperModule::FlipperModule(int Solenoid1) : RobotModule() {
    deploySol = new Solenoid(Solenoid1);
    retract();
}

void FlipperModule::deploy() {
    deploySol->Set(true);
}

void FlipperModule::retract() {
    deploySol->Set(false);
}

bool FlipperModule::isFlipperDeployed() {
    return deploySol->Get();
}
