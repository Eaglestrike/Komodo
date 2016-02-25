

#ifndef SRC_MODULES_FLIPPERMODULE_H_
#define SRC_MODULES_FLIPPERMODULE_H_

#include <WPIlib.h>
#include "RobotModule.h"

class FlipperModule: public RobotModule {
public:
	FlipperModule(int Solenoid1);
	void Deploy();
	void Retract();
	bool get();

private:
	Solenoid* deploySol;
};


#endif /* SRC_MODULES_FLIPPERMODULE_H_ */
