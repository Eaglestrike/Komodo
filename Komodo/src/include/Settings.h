/*
 * Settings.h
 *
 *  Created on: Jan 12, 2016
 *      Author: natek
 */

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

//shooter module
const double RAMPOWER = 0.60;


const double VISION_TO_GYRO = 1.24; //Ratio of vision degrees to rotation degrees (they do vary slightly)
//PID Controllers
const double RSHOOTER_CONTROLLER_P = -9.09;
const double RSHOOTER_CONTROLLER_I = 0;
const double RSHOOTER_CONTROLLER_D = -4.01;

const double PSHOOTER_CONTROLLER_P  = -8.95;
const double PSHOOTER_CONTROLLER_I = 0;
const double PSHOOTER_CONTROLLER_D = 0;

//angles
const double RMAXIMUM_ANGLE = .81;
const double RMINIMUM_ANGLE = .29;

const double PMAXIMUM_ANGLE = 0.97;
const double PMINIMUM_ANGLE = 0.452;

const double RINTAKE_ANGLE = 0.79;
const double PINTAKE_ANGLE = 0.79;

const double LEVEL_ANGLE  = 0.7281;

const double RSHOOT_ANGLE = 0.342551;
const double RSHOOT = .31;
const double SHOOT1 = .34;
const double SHOOT2 = .32;

//CAN values
const int SHOOTERMOTOR1 = 7;
const int SHOOTERMOTOR2 = 8;
const int ANGLEMOTOR = 4;

const int POT = 0;

//drive
const double RDRIVE_CONTROLLER_P = -0.012;
const double RDRIVE_CONTROLLER_I = 0;
const double RDRIVE_CONTROLLER_D = 0;

const double RANGLE_CONTROLLER_P = 0.0441;
const double RANGLE_CONTROLLER_I = 0;
const double RANGLE_CONTROLLER_D = 0.09;

const double PANGLE_CONTROLLER_P = 0.025;
const double PANGLE_CONTROLLER_I = 0;
const double PANGLE_CONTROLLER_D = 0;

const int DRIVE_LEFT1 = 2;
const int DRIVE_LEFT2 = 5;
const int DRIVE_RIGHT1 = 1;
const int DRIVE_RIGHT2 = 6;

const int DRIVE_ENCODER_1_A = 5;
const int DRIVE_ENCODER_1_B = 6;
const int DRIVE_ENCODER_2_A = 7;
const int DRIVE_ENCODER_2_B = 8;
const double TICKS_PER_INCH = 1.92;
const double DISTANCE1 = 169;
const double DISTANCE2 = 174;
const double DISTANCE3 = 252;
//intake

const int INTAKE_MOTOR_FORWARD = 3;
const int INTAKE_MOTOR_SIDEWAYS = 9;

const int TOMOHAWKS = 2;
const int INTAKE_SOL = 0;
const int SHOOTER_SOL = 1;


#endif /* SRC_SETTINGS_H_ */