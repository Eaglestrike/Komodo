/*
 * FlipperModule.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: natek
 */

FlipperModule::FlipperModule(int Solenoid1, int Solenoid2) {
	deploySol = new Solenoid(Solenoid1);
	retractSol = new Solenoid(Solenoid2);
}

void FlipperModule::Deploy() {
	deploySol->Set(true);
	retractSol->Set(false);
}

void FlipperModule::Retract() {
	deploySol->Set(false);
	retractSol->Set(true);
}
