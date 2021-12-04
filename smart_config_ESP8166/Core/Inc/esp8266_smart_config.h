#ifndef _ESP8266_SMART_CONFIG_H_
#define _ESP8266_SMART_CONFIG_H_

/************************************ DEFINE *********************************************/
#define BUFF_MAX 500
#define ENTER_CHAR 13

/************************************ ENUM *********************************************/
typedef enum
{
	esp8266_init_success,
	esp8266_init_fail,
	esp8266_init_fail_CWMODE,
	esp8266_init_fail_CIPMUX,
	esp8266_init_fail_CWSAP,
	esp8266_init_fail_CIPAP,
	esp8266_init_fail_CIPSERVER
} esp8266_init_typeDef;

/************************************ FUNCTIONS *********************************************/
/* Init esp8266 */
esp8266_init_typeDef esp8266_init(void);
/* Diasplay data */
bool esp8266_display(void);
/* Clear buff */
void esp_clear_buff(uint32_t length);

#endif /* _ESP8266_SMART_CONFIG_H_ */
