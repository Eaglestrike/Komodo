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

//PID Controllers
//#define RSHOOTER_CONTROLLER_P -7.7
//#define RSHOOTER_CONTROLLER_I 0
//#define RSHOOTER_CONTROLLER_D -0.7

#define RSHOOTER_CONTROLLER_P -7.2
#define RSHOOTER_CONTROLLER_I 0
#define RSHOOTER_CONTROLLER_D 0
#define RBACK_COURT 0.30

#define AUTOSHOT 0.298
#define RINTAKESHOOTSIDE 0.55
#define PINTAKESHOOTSIDE 0

#define PSHOOTER_CONTROLLER_P -7.2
#define PSHOOTER_CONTROLLER_I 0
#define PSHOOTER_CONTROLLER_D 0

//angles
#define RMAXIMUM_ANGLE .81
#define RMINIMUM_ANGLE .29

#define PMAXIMUM_ANGLE 0.97
#define PMINIMUM_ANGLE 0.452

#define RINTAKE_ANGLE 0.802551
#define PINTAKE_ANGLE 0.963

#define LEVEL_ANGLE 0.7281
#define PLEVEL_ANGLE 0.8921

#define RSHOOT_ANGLE 0.322551
#define RSHOOT .31
#define RSHOOT1 .324
//101 to
#define RSHOOT2 .334
// .35, .344, .349

#define PSHOOT_ANGLE 0.475551
#define PSHOOT_ANGLE1 0.48
#define PSHOOT 0.4620
#define PSHOOT1 0.4561
#define PSHOOT3 0.501

//CAN values
#define SHOOTERMOTOR1 7
#define SHOOTERMOTOR2 8
#define ANGLEMOTOR 4

#define POT 0

#define DISTANCE_PER_PULSE .04477


//drive
#define RDRIVE_CONTROLLER_P 0.012
#define RDRIVE_CONTROLLER_I 0
#define RDRIVE_CONTROLLER_D 0

#define PDRIVE_CONTROLLER_P 0.021
#define PDRIVE_CONTROLLER_I 0
#define PDRIVE_CONTROLLER_D 0

#define RANGLE_CONTROLLER_P 0.04
#define RANGLE_CONTROLLER_I 0
#define RANGLE_CONTROLLER_D 0

#define PANGLE_CONTROLLER_P 0.025
#define PANGLE_CONTROLLER_I 0
#define PANGLE_CONTROLLER_D 0

#define DRIVE_LEFT1 2
#define DRIVE_LEFT2 5
#define DRIVE_RIGHT1 1
#define DRIVE_RIGHT2 6

#define DRIVE_ENCODER_1_A 5
#define DRIVE_ENCODER_1_B 6
#define DRIVE_ENCODER_2_A 0
#define DRIVE_ENCODER_2_B 1
#define TICKS_PER_INCH 13.479
#define DISTANCE1 60
#define DISTANCE2 72
#define DISTANCE3 103
#define DISTANCE4 174
#define DISTANCE5 209
#define PDISTANCE 56.5
#define PDISTANCE2 104
#define PDISTANCE3 91
#define PDISTANCE4 174

//intake

#define INTAKE_MOTOR 3

#define TOMOHAWKS 2
#define INTAKE_SOL 0
#define SHOOTER_SOL 1
#define INTAKE_MOTOR_2 9


#endif /* SRC_SETTINGS_H_ */
