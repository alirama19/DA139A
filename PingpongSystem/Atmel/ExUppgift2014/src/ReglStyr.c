/*
* PWMSet.c
*
* Created: 2015-12-10 16:04:29
*  Author: Ali and Matko
*/
#include <asf.h>
#include "ReglStyr.h"
#include "allVariables.h"
#include "inttypes.h"
#include "ADCSet.h"
#include "UARTSet.h"

void PIDTask (void *pvParameters)
{

	// Pause here untill Matlab sent values
	xSemaphoreTake(sem, portMAX_DELAY);
	xSemaphoreTake(sem, portMAX_DELAY);

	portTickType xLastWakeTime = xTaskGetTickCount();
	const portTickType xFrequency =  DTIME_MS; // Run between 50-100ms

	vTaskDelay(1000);

	for(;;){
		vTaskDelayUntil(&xLastWakeTime,xFrequency);	// Samplingstid
		PIDReglering(); // Run PID
	}
}


/* PID Funktion */
void PIDReglering(void){

	// moving average filter of sampled sensor values
	uint32_t valuesTotal = 0;
	for(int i = 0; i<FILTER_AVERAGE_SAMPLES-1; i++){
		valuesTotal += ADCReadSensor();
	}
	distanceSensor = valuesTotal / FILTER_AVERAGE_SAMPLES;

	// Propertionell-del
	error = (setPoint - distanceSensor);

	// Integrerande-del
	sumOfError = (double)sumOfError + (double)((double)error*(double)DT_SECONDS);
	double I_Output;
	if(iValue == 0)
	{
		I_Output = 0;
		} else {
		I_Output = (double)iValue*sumOfError;
	}

	// Deriverande-del
	double D_Output;

	if(error == 1)
	{
		D_Output = 0;
		} else {
		D_Output = (double)((double)((double)dValue*(double)(error - lastError))/(double)DT_SECONDS);
	}
	lastError = error;

	// Add up P, I and D outputs
	output_value = (pValue*error)+I_Output+D_Output;
	
	// Protection vs overflow/underflow
	if (output_value < PID_PWM_MIN)
	{
		output_value = PID_PWM_MIN;
	}
	else if (output_value > PID_PWM_MAX)
	{
		output_value = PID_PWM_MAX;
	}

	// Write PID value to PWM
	PWMDutyCycle(output_value);
}


void PWMSetup()
{
	pmc_enable_periph_clk(ID_PWM);
	pwm_channel_disable(PWM, PWM_CHANNEL_6);
	pwm_clock_t pwm_clock ={
		.ul_clka = 10*9,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &pwm_clock);

	pwm_channel.alignment = PWM_ALIGN_LEFT;
	pwm_channel.polarity = PWM_LOW;
	pwm_channel.ul_prescaler = PWM_CMR_CPRE_CLKA;
	pwm_channel.ul_duty = 0;
	pwm_channel.ul_period = 999;
	pwm_channel.channel = PWM_CHANNEL_6;
	pwm_channel_init(PWM, &pwm_channel);
	pwm_channel_enable(PWM, PWM_CHANNEL_6);
}

void PWMDutyCycle(int value){
	pwm_channel_update_duty(PWM, &pwm_channel, value);
}

void shieldInit()
{
	ioport_set_pin_dir(PIO_PC21_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PC21_IDX, LOW);
	ioport_set_pin_dir(PIO_PD8_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PD8_IDX, HIGH);
}
