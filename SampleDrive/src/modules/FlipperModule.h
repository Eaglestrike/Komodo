/*
 * FlipperModule.h
 *
 *  Created on: Jan 23, 2016
 *      Author: natek
 */

#ifndef SRC_MODULES_FLIPPERMODULE_H_
#define SRC_MODULES_FLIPPERMODULE_H_

#include <WPILib.h>

class FlipperModule {
public:
	FlipperModule(int Solenoid1, int Solenoid2);
	void Deploy();
	void Retract();

private:
	Solenoid* deploySol;
	Solenoid* retractSol;
};


#endif /* SRC_MODULES_FLIPPERMODULE_H_ */
