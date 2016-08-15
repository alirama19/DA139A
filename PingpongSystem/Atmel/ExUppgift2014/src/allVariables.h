/*
* allVariables.h
*
* Created: 2015-12-10 16:04:29
*  Author: Ali and Matko
*/


#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

//Min/Max for PWM 

#define PID_PWM_MIN 0
#define PID_PWM_MAX 999

// Setpoint ADC to centimeter
#define CENTIMETER_10 3500
#define CENTIMETER_20 1740
#define CENTIMETER_30 1000
#define CENTIMETER_40 790
#define CENTIMETER_50 770
#define CENTIMETER_DEFAULT 1000

extern xSemaphoreHandle sem;

/* PID / UART */
// Variables bellow are used for both PID calculation and storing values sent from Matlab
extern double pValue;
extern double iValue;
extern double dValue;
extern int32_t setPoint;

// dt �r 50ms eller 0.05s
#define DTIME_MS 50
#define DT_SECONDS 0.05

// Convert ms to s
//#define DTIME_SECONDS DTIME_MS/1000

extern uint8_t isMatlab;
extern int32_t output_value;
extern int32_t distanceSensor;
extern int32_t error;
extern int32_t sumOfError;
extern int32_t lastError;

/* PWM */
pwm_channel_t pwm_channel;

#endif /* GLOBAL_VARIABLES_H_ */
