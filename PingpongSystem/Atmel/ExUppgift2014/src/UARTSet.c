/*
* UARTSet.c
*
* Created: 2015-12-10 16:04:29
*  Author: Ali and Matko
*/

#include <asf.h>
#include "UARTSet.h"
#include "uart.h"
#include "conf_uart_serial.h"
#include "allVariables.h"
#include "ADCSet.h"


void ComTask (void *pvParameters)
{
	getPIDValues(); // Get Matlab Values 

	xSemaphoreGive(sem); //  Start PIDTask 

	for(;;){

		// Wait here for Matlab to send signal its ready to receive values
		while (!uart_is_rx_ready (CONF_UART)){
			vTaskDelay(1);
		};

		// Clear RX buffer
		while (uart_is_rx_ready (CONF_UART)){
			uint8_t clearBuffer = 0;
			uart_read(CONF_UART, &clearBuffer);
		};

		sendValues(); // Send values to MATLAB for graphing
	}
}


void configure_console(void){
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
	.paritytype = CONF_UART_PARITY};
	// Configure console UART.
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	// configure RX0 pin as pull-up otherwise it won't receive anything (only receive 0)
	ioport_set_pin_mode(PIO_PA8_IDX, IOPORT_MODE_PULLUP);
}

/* Function that receives values from Matlab */
void getPIDValues()
{
	// divier which is used to decode encoded doubles sent from Matlab
	const uint8_t divider = 10;

	isMatlab = 0; // 1 for matlab, 0 for terminal debugging
	uint16_t kP_Gain_temp = 0;
	uint16_t kP_Gain_temp2 = 0;
	uint16_t kI_Gain_temp = 0;
	uint16_t kD_Gain_temp = 0;
	uint16_t setPointCm = 0;
	while (!uart_is_rx_ready (CONF_UART)){
		vTaskDelay(1);
	}
	uart_read(CONF_UART, &isMatlab);
	if(isMatlab == 48){ // Receive 48 because its char, 48 = 0
		isMatlab = 0; // save 0 to indicate is not Matlab COM
		printf("Terminal debugging enabled\n");
		// Set debugging values
		kP_Gain_temp = KP_GAIN_DEBUGGING;
		kI_Gain_temp = KI_GAIN_DEBUGGING;
		kD_Gain_temp = KD_GAIN_DEBUGGING;
		setPointCm = SETPOINT_DEBUGGING;
		printf("Preset values:\n");
		printf("kP: %u\n\r", (uint16_t)(kP_Gain_temp));
		printf("kI: %u\n\r", (uint16_t)(kI_Gain_temp));
		printf("kD: %u\n\r", (uint16_t)(kD_Gain_temp));
		printf("SetpointCm: %u\n\r", setPointCm);
		} else {
		while (!uart_is_rx_ready (CONF_UART)){
			vTaskDelay(1);
		}

		uart_read(CONF_UART, &kP_Gain_temp);
		while (!uart_is_rx_ready (CONF_UART)){
			vTaskDelay(1);
		}
		uart_read(CONF_UART, &kI_Gain_temp);
		while (!uart_is_rx_ready (CONF_UART)){
			vTaskDelay(1);
		};
		uart_read(CONF_UART, &kD_Gain_temp);
		while (!uart_is_rx_ready (CONF_UART)){
			vTaskDelay(1);
		};
		uart_read(CONF_UART, &setPointCm);
	}

	//Convert to correct data types
	kP = (double) ((double) kP_Gain_temp / divider);
	kI = (double) (kI_Gain_temp / divider);
	kD = (double) (kD_Gain_temp / divider);

	switch(setPointCm){
		case 10 :
		setPoint = CENTIMETER_10;
		break;

		case 20:
		setPoint = CENTIMETER_20;
		break;

		case 30 :
		setPoint = CENTIMETER_30;
		break;

		case 40 :
		setPoint = CENTIMETER_40;
		break;

		case 50 :
		setPoint = CENTIMETER_50;
		break;

		default:
		setPoint = CENTIMETER_DEFAULT;
		break;
		printf("Invalid distance\n");
	}

	// Wait here untill start signal is sent from matlab
	while (!uart_is_rx_ready (CONF_UART)){
		vTaskDelay(1);
	};
	// Clear RX buffer
	while (uart_is_rx_ready (CONF_UART)){
		uint8_t clearBuffer = 0;
		uart_read(CONF_UART, &clearBuffer);
	};
}

/* Sends values to Matlab */
void sendValues(){
	if(!isMatlab){
		printf("Values:\n");
	}
	printf("%i\n\r", error);
	printf("%i\n\r", output_value);
	printf("%i\n\r", distanceSensor);
	printf("%i\n\r", setPoint);
}
