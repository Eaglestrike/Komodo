

#ifndef SRC_MODULES_FLIPPERMODULE_H_
#define SRC_MODULES_FLIPPERMODULE_H_

#include <WPIlib.h>

class FlipperModule {
public:
	FlipperModule(int Solenoid1);
	void Deploy();
	void Retract();
	bool get();

private:
	Solenoid* deploySol;
};


#endif /* SRC_MODULES_FLIPPERMODULE_H_ */
