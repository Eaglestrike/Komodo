/*
 * FlipperModule.cpp
 *
<<<<<<< HEAD
 *  Created on: Jan 21, 2016
 *      Author: Samsung
 */

#include <modules/FlipperModule.h>

FlipperModule::FlipperModule() {
	// TODO Auto-generated constructor stub

}

FlipperModule::~FlipperModule() {
	// TODO Auto-generated destructor stub
}

=======
 *  Created on: Jan 23, 2016
 *      Author: natek
 */

#include "FlipperModule.h"

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
>>>>>>> dbae54ae4a00404b92c45167e62093cdb6e036c0
