#ifndef SRC_MODULES_FLIPPERMODULE_H_
#define SRC_MODULES_FLIPPERMODULE_H_

#include <WPILib.h>
#include "RobotModule.h"

class FlipperModule : public RobotModule {
public:
    explicit FlipperModule(int Solenoid1);

    void Deploy();

    void Retract();

    bool get();

private:
    Solenoid *deploySol;
};


#endif /* SRC_MODULES_FLIPPERMODULE_H_ */
