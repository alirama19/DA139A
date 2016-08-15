/*
 * PWMSet.h
 *
 * Created: 2015-12-10 16:04:29
 *  Author: Ali and Matko
 */


#ifndef REGLSTYR_H_
#define REGLSTYR_H_

void PIDReglering(void);
//Defines for creation of PIDRegulationTask
#define TASK_PIDRegulation_STACK_SIZE (2048/sizeof(portSTACK_TYPE))
#define TASK_PIDRegulation_STACK_PRIORITY (2)

void PIDTask (void *pvParameters);


void PWMSetup(void);
void PWMDutyCycle(int);
void shieldInit(void);



#endif /* REGLSTYR_H_ */
