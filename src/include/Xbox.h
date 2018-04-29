/*
 * Xbox.h
 *
 *  Created on: Aug 31, 2015
 *      Author: LAHS
 */

#ifndef XBOX_H
#define XBOX_H

#include "WPILib.h"

class Xbox : Joystick {
public:

    explicit Xbox(int port);

    ~Xbox() override = default;

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

    double getLT();

    double getRT();

    double getRX();

    double getRY();

    double getLX();

    double getLY();

    void setRumble(float);

    void setRRumble(float);

    void setLRumble(float);
};


#endif
