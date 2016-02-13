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
#define RSHOOTER_CONTROLLER_P 0
#define RSHOOTER_CONTROLLER_I 0
#define RSHOOTER_CONTROLLER_D 0

#define PSHOOTER_CONTROLLER_P 0
#define PSHOOTER_CONTROLLER_I 0
#define PSHOOTER_CONTROLLER_D 0

//angles
#define RMAXIMUM_ANGLE 0
#define RMINIMUM_ANGLE 0

#define PMAXIMUM_ANGLE 0
#define PMINIMUM_ANGLE 0

#define RINTAKE_ANGLE 0

#define PINTAKE_ANGLE 0

//CAN values
#define SHOOTERMOTOR1 0
#define SHOOTERMOTOR2 0
#define ANGLEMOTOR 0


//drive
#define DRIVE_CONTROLLER_P 0.017
#define DRIVE_CONTROLLER_I 0
#define DRIVE_CONTROLLER_D 0.052

#define DRIVE_LEFT1 3
#define DRIVE_LEFT2 2
#define DRIVE_RIGHT1 0
#define DRIVE_RIGHT2 1
#define DRIVE_ENCODER_1_A 0
#define DRIVE_ENCODER_1_B 1
#define DRIVE_ENCODER_2_A 4
#define DRIVE_ENCODER_2_B 5
#define TICKS_PER_INCH 13.479

//intake
#define INTAKE_SOL1 0
#define INTAKE_SOL2 0

#define INTAKE_MOTOR 0

#endif /* SRC_SETTINGS_H_ */
