

#ifndef SRC_MODULES_FLIPPERMODULE_H_
#define SRC_MODULES_FLIPPERMODULE_H_

#include <WPIlib.h>
#include "RobotModule.h"

class FlipperModule: public RobotModule{
public:
	FlipperModule(int Solenoid1, int Solenoid2);
	virtual ~FlipperModule();
	void Deploy();
	void Retract();

private:
	Solenoid* deploySol;
	Solenoid* retractSol;
};


#endif /* SRC_MODULES_FLIPPERMODULE_H_ */
