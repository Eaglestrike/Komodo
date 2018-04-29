#include <Xbox.h>

Xbox::Xbox(int port) : Joystick(port) {}

bool Xbox::getA() {
    return GetRawButton(1);
}

bool Xbox::getB() {
    return GetRawButton(2);
}

bool Xbox::getX() {
    return GetRawButton(3);
}

bool Xbox::getY() {
    return GetRawButton(4);
}

bool Xbox::getRB() {
    return GetRawButton(6);
}

bool Xbox::getLB() {
    return GetRawButton(5);
}

bool Xbox::getStart() {
    return GetRawButton(8);
}

bool Xbox::getBack() {
    return GetRawButton(7);
}

bool Xbox::getR3() {
    return GetRawButton(10);
}

bool Xbox::getL3() {
    return GetRawButton(9);
}


double Xbox::getLT() {
    return GetRawAxis(3);
}

double Xbox::getRT() {
    return GetRawAxis(3);
}

double Xbox::getRX() {
    return GetRawAxis(4);
}

double Xbox::getRY() {
    return -GetRawAxis(5);
}

double Xbox::getLX() {
    return GetX();
}

double Xbox::getLY() {
    return -GetY();
}

void Xbox::setRumble(float rumble) {
    SetRumble(kLeftRumble, rumble);
    SetRumble(kRightRumble, rumble);
}

void Xbox::setLRumble(float rumble) {
    SetRumble(kLeftRumble, rumble);
}

void Xbox::setRRumble(float rumble) {
    SetRumble(kRightRumble, rumble);
}
