/*
 * Xbox.h
 *
 *  Created on: Aug 31, 2015
 *      Author: LAHS
 */

#ifndef XBOX_H
#define XBOX_H

#include "WPILib.h"

class Xbox : public Joystick {
private:
    bool buttonStatus[10] = {false, false, false, false, false, false, false, false, false, false};
public:
    enum button {
        X = 3,
        Y = 4,
        A = 1,
        B = 2,
        RB = 6,
        LB = 5,
        Start = 8,
        Back = 7
    };

    Xbox(int port);

    ~Xbox() {};

    bool getA();

    bool getB();

    bool getX();

    bool getY();

    bool getButtonPress(button axis);

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
