/*
 * Settings.h
 *
 *  Created on: Jan 12, 2016
 *      Author: natek
 */

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

//shooter module

//PID Controllers
#define RSHOOTER_CONTROLLER_P -7.4
#define RSHOOTER_CONTROLLER_I 0
#define RSHOOTER_CONTROLLER_D 0

#define PSHOOTER_CONTROLLER_P 0
#define PSHOOTER_CONTROLLER_I 0
#define PSHOOTER_CONTROLLER_D 0

//angles
#define RMAXIMUM_ANGLE .58
#define RMINIMUM_ANGLE .04

#define PMAXIMUM_ANGLE 0
#define PMINIMUM_ANGLE 0

#define RINTAKE_ANGLE .56

#define PINTAKE_ANGLE 0
#define RSHOOT_ANGLE .08
//CAN values
#define SHOOTERMOTOR1 7
#define SHOOTERMOTOR2 8
#define ANGLEMOTOR 4

#define POT 0


//drive
#define DRIVE_CONTROLLER_P 0.017
#define DRIVE_CONTROLLER_I 0
#define DRIVE_CONTROLLER_D 0.052

#define DRIVE_LEFT1 2
#define DRIVE_LEFT2 5
#define DRIVE_RIGHT1 1
#define DRIVE_RIGHT2 6

#define DRIVE_ENCODER_1_A 0
#define DRIVE_ENCODER_1_B 1
#define DRIVE_ENCODER_2_A 4
#define DRIVE_ENCODER_2_B 5
#define TICKS_PER_INCH 13.479

//intake

#define INTAKE_MOTOR 3

#define TOMOHAWKS 2
#define INTAKE_SOL 0
#define SHOOTER_SOL 1


#endif /* SRC_SETTINGS_H_ */
