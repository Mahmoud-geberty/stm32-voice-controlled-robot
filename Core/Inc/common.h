/*
 * common.h
 *
 *  Created on: Dec 1, 2022
 *      Author: amera
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

//#include "pca9685.h"
#include "servo_driver.h"
#include "main.h"

#define LowerLeg 0.145
#define UpperLeg 0.122
#define RADTODEG(x) x*57.2925

#define FLHipmin 110
#define FLHipmax 500

#define FLKneemin 130
#define FLKneemax 350

enum { FLShoulder = 0, FLHip, FLKnee, FRShoulder, FRHip, FRKnee, BLShoulder, BLHip, BLKnee, BRShoulder, BRHip, BRKnee};

typedef struct {
float des_hip_angle;
float des_knee_angle;
float des_shoulder_angle;

float hip_angle;
float knee_angle;
float shoulder_angle;

float x_angle,x_height;
float y_angle,y_height;


uint8_t dir;
}LEG_t;

extern Srv_Drv_t servodriver;
extern uint32_t ADCValue[4];
extern float height;

extern char data[100];
extern char control;
extern int smoothdelay, servostep;

extern float x_distance;
extern LEG_t legFL,legFR,legBL,legBR;
extern void SysInit();
extern void ControlLoop();
void LegUpdate(LEG_t *leg,float x_dist, float y_dist, float height, float roll, float pitch, float yaw);
void LegInit(LEG_t *leg, uint8_t dir);
void LEGStepLoop(LEG_t *leg);
extern void SetSmoothpeed();
extern void SetSmoothSpeed(float SmoothSpeed);
extern uint16_t MAP_Angle2Pulse(float input, float AngleMin, float AngleMax, int PulseMin, int PulseMax);
void StepLoop(float desired, float *current);
#endif /* INC_COMMON_H_ */
