/*
 * c3_fieldfare.c
 *
 * Created: 04-Dec-15 7:53:39 PM
 * Author : emon1
 */ 

#include "hal_common.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "rtc.h"
#include "clock_system.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "serial_stdio.h"
#include "ssd1306.h"
#include "gfx_mono.h"
#include "esp8266.h"


void lcd_line_print(int line, char *str){
	ssd1306_clear();
	ssd1306_set_page_address(line);
	ssd1306_set_column_address(0);
	ssd1306_write_text(str);
}




int main(void)
{
	//
	rtc_init();
	
	
	//changing the clock to 32MHz
	enable_oscillator(OSC_RC32MEN_bm);
	sysclk_prescaler(CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	sysclk_source(CLK_SCLKSEL_RC32M_gc);

	usart_config(&USARTC0,&PORTC,USART_CMODE_ASYNCHRONOUS_gc,USART_PMODE_DISABLED_gc,USART_CHSIZE_8BIT_gc,false,false);
	usart_baud(&USARTC0,115200,-3);
	FILE my_stdio;
	create_file_stream(&my_stdio,&USARTC0_putchar,&USARTC0_getchar);
	set_stdio_stream(&my_stdio);



	port_direction_set_mask(&PORTR,1<<0);
	
	uint8_t page;
	uint8_t column;
	
	gfx_mono_set_framebuffer(framebuffer);
	
	ssd1306_init();
	//ssd1306_clear();
	
	ssd1306_set_display_start_line_address(0);
	

	for (page = 0; page < GFX_MONO_LCD_PAGES; page++) {
		for (column = 0; column < GFX_MONO_LCD_WIDTH; column++) {
			gfx_mono_ssd1306_put_byte(page, column, 0x00, 1);
		}
	}
	
	ssd1306_set_page_address(0);
	ssd1306_write_text("EMON");
	
	rtc_ms_delay(1000);
	
	lcd_line_print(0,"initializing wifi");
	esp8266_init();


	esp8266_wifi_mode(1);
	esp8266_check_wifi_mode();
		
	esp8266_multiple_connection_enable(1);
		
	ESP8266_available_AP_t myAPdata[10];
	int n = esp8266_list_access_points(myAPdata);
		
	for(int i=0;i<n;i++){
	printf("AP INFO :%d,%s,%d,%s\n",myAPdata[i].ecn, myAPdata[i].ssid,myAPdata[i].sig_strength,myAPdata[i].mac );
	}
		
	int ssid_found_flag = 0;
	for(int i=0;i<n;i++){
		if(!strcmp(myAPdata[i].ssid,"Emon")){
			printf("FOUND SSID : Emon\n");
			esp8266_connect_access_points(myAPdata[i].ssid,"19031992");
			ssid_found_flag = 1;
		}
		else{
				
		}
	}
	if(ssid_found_flag!= 1){
		printf("could not find SSID\n");
	}
		
	char connected_ssid[32];
	esp8266_check_connected_access_points(connected_ssid);
	printf("the connected SSID is %s\n",connected_ssid);
	//esp8266_disconnect_access_points();
	
	
	char IPaddr[20];
	char MACaddr[30];	
	esp8266_get_IP(IPaddr);
	esp8266_get_MAC(MACaddr);
	printf("YOU IP IS %s and MAC is %s\n",IPaddr,MACaddr);
	lcd_line_print(1, IPaddr);
	
	
	esp8266_set_server(8000);
	esp8266_check_server_timeout();
	//esp8266_close_server();
// 	esp8266_start_connection(0, TCP, "192.168.0.102", 8000);
// 	rtc_ms_delay(4000);
// 	esp8266_close_connection(0);
// 	rtc_ms_delay(4000);
// 
// 	esp8266_start_connection(0, TCP, "192.168.0.102", 8000);
// 	for(int i=0;i<100;i++){
// 			esp8266_send_data_to_id(0,strlen("EMON\n"),(uint8_t *) "EMON\n");
// 	}
	
	
	
	
	printf("DONE\n");
    while (1) 
    {
		esp8266_check_connection_status(NULL);
		rtc_ms_delay(1000);
// 	
// 		char cmd[10];
// 				printf("enter command\n");
// 				scanf("%s",cmd);
// 				
// 				if(!strcmp("lcd_on",cmd)){
// 					printf("LCD is ON\n");
// 					ssd1306_display_on();
// 				}
// 				else if(!strcmp("lcd_off",cmd)){
// 					printf("LCD is OFF\n");
// 					ssd1306_display_off();
// 				}
// 				else if(!strcmp("lcd_inv_on",cmd)){
// 					printf("LCD inversion is ON\n");
// 					ssd1306_display_invert_enable();
// 				}
// 				else if(!strcmp("lcd_inv_off",cmd)){
// 					printf("LCD inversion is OFF\n");
// 					ssd1306_display_invert_disable();
// 				}
// 				
// 				else if(!strcmp("wifi_version",cmd)){
// 					printf("ESP8266 version\n");
// 					fprintf(&esp8266_stream,"AT+GMR\n\r");
// 					esp8266_receive(2000);
// 				}
// 				else if(!strcmp("wifi_reset",cmd)){
// 					printf("ESP8266 reset\n");
// 					fprintf(&esp8266_stream,"AT+RST\n\r");
// 					esp8266_receive(2000);
// 				}
// 				else if(!strcmp("wifi_list_ap",cmd)){
// 					printf("setting CW mode\n");
// 					fprintf(&esp8266_stream,"AT+CWMODE=1\n\r");
// 					esp8266_receive(2000);
// 					
// 					printf("setting CIPMUX mode\n");
// 					fprintf(&esp8266_stream,"AT+CIPMUX=1\n\r");
// 					esp8266_receive(2000);
// 					
// 					printf("Listing Available AP\n");
// 					fprintf(&esp8266_stream,"AT+CWLAP\n\r");
// 					esp8266_receive(8000);
// 				}
// 				else if(!strcmp("wifi_join_ap",cmd)){
// 
// 					printf("Connecting to AP\n");
// 					fprintf(&esp8266_stream,"AT+CWJAP=\"Emon\",\"19031992\"\n\r");
// 					esp8266_receive(10000);
// 				}
// 				else if(!strcmp("wifi_server",cmd)){
// 
// 					printf("Connecting to AP\n");
// 					fprintf(&esp8266_stream,"AT+CIPSERVER=1,8000\n\r");
// 					esp8266_receive(10000);
// 				}
// 				else if(!strcmp("wifi_check",cmd)){
// 
// 					printf("Checking connection\n");
// 					fprintf(&esp8266_stream,"AT+CIFSR\n\r");
// 					esp8266_receive(2000);
// 				}					
// 				else if(!strcmp("rtc",cmd)){
// 					printf("RTC tick : %lu\n",rtc_tick);
// 				}
// 
// 				else if(!strcmp("led_tgl",cmd)){
// 					port_out_value_toggle_mask(&PORTR,1<<0);
// 				}
// 				
// 				else{
// 					printf("unknown command\n");
// 				}

				
				
				esp8266_wait_receive();

}
	
}

