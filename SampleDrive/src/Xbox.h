/*
 * Xbox.h
 *
 *  Created on: Aug 31, 2015
 *      Author: LAHS
 */

#ifndef XBOX_H
#define XBOX_H

#include "WPILib.h"

class Xbox: public Joystick{

public:

	Xbox(int port);
	~Xbox(){};

	bool getA();
	bool getB();
	bool getX();
	bool getY();

	bool getRB();
	bool getLB();

	bool getStart();
	bool getBack();

	bool getR3();
	bool getL3();

	float getLT();
	float getRT();

	float getRX();
	float getRY();
	float getLX();
	float getLY();
	float getPX();



	void setRumble(float);
	void setRRumble(float);
	void setLRumble(float);
};


#endif
