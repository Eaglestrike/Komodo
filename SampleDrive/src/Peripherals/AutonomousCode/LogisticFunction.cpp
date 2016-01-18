/*
 * LogisticFunction.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: natek
 */
#include "LogisticFunction.h"
#include <math.h>

LogisticFunction::LogisticFunction(double maxTime, double maxDistance) {
	errorD = 0.5; //how many units we can be away from target distance
	D = maxDistance + errorD; //Adjust the target distance so we stop at the target
	maxT = maxTime;
	A = (log(errorD/D)/maxT); //from (D-E) = D(1-e^(-AT))
}

double LogisticFunction::getDistance(double time) {
	return D*(1-exp(-A*time));
}
