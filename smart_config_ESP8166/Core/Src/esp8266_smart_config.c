#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "delay_timer.h"
#include "esp8266_smart_config.h"

/************************************ VARIABLE *********************************************/
#define TIMER_WAIT 1000

/************************************ VARIABLE *********************************************/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* Wifi information */
uint8_t WIFI_NAME[100];
uint8_t WIFI_PASS[100];

/* AT command */
static uint8_t ATE0[] = "ATE0\r\n";
static uint8_t AT_CWMODE[] = "AT+CWMODE=2\r\n";
static uint8_t AT_CIPMUX[] = "AT+CIPMUX=1\r\n";
static uint8_t AT_CWSAP[] = "AT+CWSAP=\"ESP01\",\"\",5,0\r\n";
static uint8_t AT_CIPAP[] = "AT+CIPAP=\"192.168.4.1\"\r\n";
static uint8_t AT_SERVER[] = "AT+CIPSERVER=1,1234\r\n";

/* Declare variables */
uint8_t esp_buff[BUFF_MAX]; /* Buffer receive */
uint8_t esp_rx_data = 0;        /* Byte receive */
uint32_t esp_rx_index = 0;      /* Index */
bool esp_rx_done = false;   /* Receive done flag */
static bool esp_start_parse_data = false;
static bool esp_start_get_wifi = false;

/************************************ FUNCTIONS *********************************************/
/* Init esp8266 */
esp8266_init_typeDef esp8266_init(void)
{
	uint8_t step = 1;
	bool endLoop = false;
	esp8266_init_typeDef retVal = esp8266_init_fail;

	HAL_UART_Transmit(&huart2, ATE0, sizeof(ATE0), 100);	
	delay_ms(TIMER_WAIT);
	while(!endLoop)
	{
		switch(step)
		{
			case 1:
			{
				HAL_UART_Transmit(&huart2, AT_CWMODE, sizeof(AT_CWMODE), 100);
				break;
			}
			case 2:
			{
				HAL_UART_Transmit(&huart2, AT_CIPMUX, sizeof(AT_CIPMUX), 100);
				break;
			}
			case 3:
			{
				HAL_UART_Transmit(&huart2, AT_CWSAP, sizeof(AT_CWSAP), 100);
				break;
			}
			case 4:
			{
				HAL_UART_Transmit(&huart2, AT_CIPAP, sizeof(AT_CIPAP), 100);
				break;
			}
			case 5:
			{
				HAL_UART_Transmit(&huart2, AT_SERVER, sizeof(AT_SERVER), 100);
				break;
			}
			default :
				break;
		}
		delay_ms(TIMER_WAIT);

		if(esp8266_display())
		{
			step += 1;
		}
	}
	
	return retVal;
}

/* Diasplay data */
bool esp8266_display(void)
{
	uint32_t counter = 0;
	bool retVal = false;

	if(esp_rx_done)
	{
		/* Send data to terminal */
		HAL_UART_Transmit(&huart1, esp_buff, sizeof(esp_buff), 100);
		/* Verify data respond */
		if(esp_start_parse_data && !esp_start_get_wifi)
		{
			for(counter = 0; counter < sizeof(esp_buff); counter ++)
			{
				if((esp_buff[0] == 'O') && (esp_buff[1] == 'K'))
				{
					retVal = true;
				}
			}
		}
		/* Get wifi and password */
		else if(esp_start_parse_data && esp_start_get_wifi)
		{
			
		}
		
		/* Clear flag esp_rx_done */
		esp_rx_done = false;
		/* Clear buffer */
		esp_clear_buff(sizeof(esp_buff));
	}
	
	return retVal;
}

/* Clear buff */
void esp_clear_buff(uint32_t length)
{
	uint32_t counter = 0;
	
	for(counter = 0; counter < length; counter ++)
	{
		esp_buff[counter] = NULL;
	}
}
