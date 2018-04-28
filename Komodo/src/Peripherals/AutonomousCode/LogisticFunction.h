/*
 * LogisticFunction.h
 *
 *  Created on: Jan 16, 2016
 *      Author: natek
 */

#ifndef SRC_PERIPHERALS_AUTONOMOUSCODE_LOGISTICFUNCTION_H_
#define SRC_PERIPHERALS_AUTONOMOUSCODE_LOGISTICFUNCTION_H_

class LogisticFunction { //x=D(1-e^(-At))
public:
    LogisticFunction(double maxDistance, double maxTime);

    virtual ~LogisticFunction() {};

    double getDistance(double time);

    double getA();

private:
    double D;
    double A;
    double maxT;
    double errorD;
};


#endif /* SRC_PERIPHERALS_AUTONOMOUSCODE_LOGISTICFUNCTION_H_ */
