/*
* allVariables.c
*
* Created: 2015-12-10 16:04:29
*  Author: Ali and Matko
*/

#include <asf.h>
#include "allVariables.h"

xSemaphoreHandle sem = NULL;

/* PID */
// Processed values for the PID regulator, p_temp / divider ..
double pValue = 0;
double iValue = 0;
double dValue = 0;
int32_t setPoint = 0;

uint8_t isMatlab = 1; // 1 for matlab, 0 for terminal debugging
int32_t output_value = 0;
int32_t distanceSensor = 0;
int32_t error = 0;
int32_t sumOfError = 0;
int32_t lastError = 0;
