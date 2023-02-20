/*
 * common.c
 *
 *  Created on: Dec 1, 2022
 *      Author: amera
 */
#include "common.h"

Srv_Drv_t servodriver;
uint32_t ADCValue[4];
char data[100];
float height;
float x_distance;
int smoothdelay, servostep;
char control;
LEG_t legFL,legFR,legBL,legBR;

void SysInit(){

	//Peripheral Initialization
	LegInit(&legFL, 0);
	height = 0.18;
	x_distance = 0.05;
//	legFL.hip_angle = 60.0;
//	legFL.knee_angle = 71.0;

	ServoDriverInit(&servodriver, &hi2c3, 0x40);

//	vTaskSuspendAll();
//	ServoDriverSetPWM(&servodriver, FLKnee,  MAP_Angle2Pulse(knee_joint,20.0,90.0,FLKneemin,FLKneemax),0);
//	ServoDriverSetPWM(&servodriver, FLHip, MAP_Angle2Pulse(hip_joint,0.0,180.0,FLHipmin,FLHipmax),0);

//	xTaskResumeAll();

	SetSmoothSpeed(15.0);
	HAL_TIM_Base_Start_IT(&htim6);
//	HAL_UART_Receive_IT(&huart3, &control, 1);

}

uint16_t MAP_Angle2Pulse(float input, float AngleMin, float AngleMax, int PulseMin, int PulseMax)
{
	uint16_t result = ((((input - AngleMin)*(float)(PulseMax - PulseMin))/(AngleMax - AngleMin)) + PulseMin);

	if(result > PulseMax)
		result = PulseMax;
	else if(result < PulseMin)
		result = PulseMin;

	return result;
}

void ControlLoop(){
	LegUpdate(&legFL,x_distance, 0.0, height, 0.0, 0.0, 0.0);
}

void LegUpdate(LEG_t *leg,float x_dist, float y_dist, float height, float roll, float pitch, float yaw){
	leg->x_angle = atan(x_dist/height);
	leg->x_height = sqrt(pow(x_dist,2) + pow(height,2));

	leg->des_knee_angle =  RADTODEG(acos((pow(LowerLeg,2)+pow(UpperLeg,2)-pow(leg->x_height,2))/(2.0*UpperLeg*LowerLeg))) ;
	leg->des_hip_angle = 90.0 - RADTODEG(acos((pow(leg->x_height,2)+pow(UpperLeg,2)-pow(LowerLeg,2))/(2.0*UpperLeg*leg->x_height))) + RADTODEG(leg->x_angle);
}

void LegInit(LEG_t *leg, uint8_t dir){
	leg->des_hip_angle  =  60.0;
	leg->des_knee_angle  =  73.1;
	leg->hip_angle = 60.0;
	leg->knee_angle = 73.1;
	leg->dir = dir;
}

void LEGStepLoop(LEG_t *leg){
	StepLoop(leg->des_hip_angle, &(leg->hip_angle));
	StepLoop(leg->des_knee_angle, &(leg->knee_angle));
	StepLoop(leg->des_shoulder_angle, &(leg->shoulder_angle));
}

void SetSmoothSpeed(float SmoothSpeed){


	smoothdelay = (int) SmoothSpeed;

	if(smoothdelay <= 0){
		smoothdelay = 1;
	}

	servostep = (int)(1.0/SmoothSpeed);

	if(servostep <= 0){
		servostep = 1;
	}

}


void StepLoop(float desired, float *current){

	if(*current != desired){
		if(desired > *current){
			*current+=(float)servostep;
			if(*current > desired){
				*current = desired;
			}
		}else if (desired < *current){
			*current-=(float)servostep;
			if(*current < desired){
				*current = desired;
			}
		}
	}

}
