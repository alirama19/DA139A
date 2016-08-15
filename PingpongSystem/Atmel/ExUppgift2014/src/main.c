#include <asf.h>
#include "delay.h"
#include "config/conf_board.h"
#include "config/conf_clock.h "
#include "allVariables.h"
#include "ADCSet.h"
#include "ReglStyr.h"
#include "UARTSet.h"


int main (void)
{
	// Board init
	sysclk_init();
	board_init();
	ioport_init();

	delay_init();
	configure_console(); // Initialize UART

	PWMSetup();
	shieldInit();
	ADCSetup();
	
	//initialize binary semaphore
	vSemaphoreCreateBinary(sem);
	if( sem != NULL )
	{

	}
	
	/* Creating the PID regulation task */
	if (xTaskCreate(PIDTask, (const signed char * const) "PIDRegulation",
	TASK_PIDRegulation_STACK_SIZE, NULL, TASK_PIDRegulation_STACK_PRIORITY, NULL) != pdPASS)
	{
		printf("Failed to create PIDRegulationTask\r\n");
	}
	
	/* Creating the serial communication task */
	if (xTaskCreate(ComTask, (const signed char * const) "SerialComTask",
	TASK_SerialComTask_STACK_SIZE, NULL, TASK_SerialComTask_STACK_PRIORITY, NULL) != pdPASS)
	{
		printf("Failed to create SerialComTask\r\n");
	}
	
	///* Start the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();
	printf("Insufficient RAM\r\n");
	while(1);
	
}