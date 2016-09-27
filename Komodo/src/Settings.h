/*
 * Settings.h
 *
 *  Created on: Jan 12, 2016
 *      Author: natek
 */

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

//shooter module
#define RAMPOTPOSITION 0.4076
#define RAMPOWER 0.60


#define VISION_TO_GYRO 1.24 //Ratio of vision degrees to rotation degrees (they do vary slightly)
//PID Controllers
//#define RSHOOTER_CONTROLLER_P -7.7
//#define RSHOOTER_CONTROLLER_I 0
//#define RSHOOTER_CONTROLLER_D -0.7

#define RSHOOTER_CONTROLLER_P -9.09
#define RSHOOTER_CONTROLLER_I 0
#define RSHOOTER_CONTROLLER_D -4.01
#define RBACK_COURT 0.30

#define AUTOSHOT 0.298
#define RINTAKESHOOTSIDE 0.55
#define PINTAKESHOOTSIDE 0

#define PSHOOTER_CONTROLLER_P -8.95
#define PSHOOTER_CONTROLLER_I 0
#define PSHOOTER_CONTROLLER_D 0

//angles
#define RMAXIMUM_ANGLE .81
#define RMINIMUM_ANGLE .29

#define PMAXIMUM_ANGLE 0.97
#define PMINIMUM_ANGLE 0.452

#define RINTAKE_ANGLE 0.79
#define PINTAKE_ANGLE 0.79

#define LEVEL_ANGLE 0.7281
#define PLEVEL_ANGLE 0.8921

#define RSHOOT_ANGLE 0.342551
#define RSHOOT .31
#define RSHOOT1 .304
//101 to
#define RSHOOT2 .317
// .35, .344, .349

#define SHOOT1 .34
#define SHOOT2 .32

//CAN values
#define SHOOTERMOTOR1 7
#define SHOOTERMOTOR2 8
#define ANGLEMOTOR 4

#define POT 0

#define DISTANCE_PER_PULSE .04477


//drive
#define RDRIVE_CONTROLLER_P -0.012
#define RDRIVE_CONTROLLER_I 0
#define RDRIVE_CONTROLLER_D 0

#define RANGLE_CONTROLLER_P 0.0441
#define RANGLE_CONTROLLER_I 0
#define RANGLE_CONTROLLER_D 0.09

#define PANGLE_CONTROLLER_P 0.025
#define PANGLE_CONTROLLER_I 0
#define PANGLE_CONTROLLER_D 0

#define DRIVE_LEFT1 2
#define DRIVE_LEFT2 5
#define DRIVE_RIGHT1 1
#define DRIVE_RIGHT2 6

#define DRIVE_ENCODER_1_A 5
#define DRIVE_ENCODER_1_B 6
#define DRIVE_ENCODER_2_A 7
#define DRIVE_ENCODER_2_B 8
#define TICKS_PER_INCH 1.92
#define DISTANCE1 169
#define DISTANCE2 174
#define DISTANCE3 252
//intake

#define INTAKE_MOTOR 3

#define TOMOHAWKS 2
#define INTAKE_SOL 0
#define SHOOTER_SOL 1
#define INTAKE_MOTOR_2 9


#endif /* SRC_SETTINGS_H_ */
