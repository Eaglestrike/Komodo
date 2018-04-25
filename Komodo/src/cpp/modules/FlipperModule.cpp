
#include <modules/FlipperModule.h>

FlipperModule::FlipperModule(int Solenoid1) : RobotModule("FlipperModule") {
	deploySol = new Solenoid(Solenoid1);
}

void FlipperModule::Deploy() {
	deploySol->Set(true);
}

void FlipperModule::Retract() {
	deploySol->Set(false);
}

bool FlipperModule::get() {
	return deploySol->Get();
}
