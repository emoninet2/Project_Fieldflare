/*
 * esp8266nodemcu.h
 *
 * Created: 14-Feb-16 4:10:10 AM
 *  Author: emon1
 */ 


#ifndef ESP8266NODEMCU_H_
#define ESP8266NODEMCU_H_


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static void tokenize(char * command, int * arg_count, const char *delim, int max_tokens, char *arg_strings[]){

	int arg_index = 0;
	char *pch;
	pch = strtok(command,delim);
	while(pch != NULL){
		arg_strings[arg_index++] = pch;
		//arg_index++;
		if(arg_index >max_tokens)break;
		pch = strtok(NULL,delim);
	}
	*arg_count = arg_index;
}


volatile char esp8266_rx_buff[500];
volatile int esp8266_rx_buff_charcount;
volatile char *esp8266_rx_buff_write_p = esp8266_rx_buff;
volatile char *esp8266_rx_buff_read_p = esp8266_rx_buff;
volatile bool esp8266_buff_write_enabled = 0;
volatile bool esp8266_buff_receive_complete = 0;


volatile rtctick_t esp8266_rx_timesnap = 0;



#define ESP8266_Wait_ms(ms) rtc_ms_delay(ms);
#define ESP8266_UART_Putchar(data) usart_transmit_by_val(&USARTE0,data)

void esp8266_serial_init(){
	usart_config(&USARTE0,&PORTE,USART_CMODE_ASYNCHRONOUS_gc,USART_PMODE_DISABLED_gc,USART_CHSIZE_8BIT_gc,false,false);
	usart_baud(&USARTE0,9600,-3);
	usart_set_interrupt_level(&USARTE0, USART_RXCINTLVL_HI_gc, USART_TXCINTLVL_OFF_gc, USART_DREINTLVL_OFF_gc);
	//create_file_stream(&esp8266_stream,&USARTE0_putchar,&USARTE0_getchar);
}

// void esp8266_send(char *data){
// 	for(int i=0;i<strlen(data);i++){
// 		ESP8266_UART_Putchar(*(data + i));
// 	}
// 	ESP8266_UART_Putchar('\r');
// 	ESP8266_UART_Putchar('\n');
// }


// void esp8266_rx_buff_read(char *dataout){
// 	if(esp8266_buff_receive_complete){
// 		strcpy(dataout, (const char*) esp8266_rx_buff);
// 		//printf("%s",esp8266_rx_buff);
// 		esp8266_rx_buff_write_p = esp8266_rx_buff;
// 		esp8266_rx_buff_read_p = esp8266_rx_buff;
// 		esp8266_buff_receive_complete = 0;
// 	}
// }

void esp8266_rx_buff_receive(char *data){
	esp8266_rx_buff_charcount=0;
	esp8266_rx_timesnap = 0;
	esp8266_rx_buff_write_p = data;
	esp8266_buff_write_enabled = 1;
	esp8266_buff_receive_complete = 0;
	
	while(esp8266_rx_timesnap == 0);
	
	while(rtc_tick - esp8266_rx_timesnap  <= 5){;}
	
	
	esp8266_buff_write_enabled = 0;
	esp8266_buff_receive_complete = 1;
	//*esp8266_rx_buff_write_p = '\0';
	data[esp8266_rx_buff_charcount] = '\0';

	//
	
	//esp8266_rx_buff_read(data);
}

void esp8266_send_receive_timeout(char *datain, char *dataout, rtctick_t timeout_ms){
	esp8266_rx_buff_charcount=0;
	esp8266_rx_timesnap = 0;
	esp8266_rx_buff_write_p = dataout;
	esp8266_buff_write_enabled = 1;
	esp8266_buff_receive_complete = 0;

		for(int i=0;i<strlen(datain);i++){
			ESP8266_UART_Putchar(*(datain + i));
		}
		ESP8266_UART_Putchar('\r');
		ESP8266_UART_Putchar('\n');

	rtctick_t esp8266_rx_timeout_timesnap = rtc_tick;
	while(esp8266_rx_timesnap == 0);

	while(rtc_tick - esp8266_rx_timeout_timesnap <= timeout_ms){
		while(rtc_tick - esp8266_rx_timesnap  <= rtc_ms_ticks(100)){;}
	}
	

	esp8266_buff_write_enabled = 0;
	esp8266_buff_receive_complete = 1;
	//*esp8266_rx_buff_write_p = '\0';
	dataout[esp8266_rx_buff_charcount] = '\0';
}





void esp8266_send_receive(char *datain, char *dataout){
	esp8266_rx_buff_charcount=0;
	esp8266_rx_timesnap = 0;
	esp8266_rx_buff_write_p = dataout;
	esp8266_buff_write_enabled = 1;
	esp8266_buff_receive_complete = 0;

		for(int i=0;i<strlen(datain);i++){
			ESP8266_UART_Putchar(*(datain + i));
		}
		ESP8266_UART_Putchar('\r');
		ESP8266_UART_Putchar('\n');


	while(esp8266_rx_timesnap == 0);

	while(rtc_tick - esp8266_rx_timesnap  <= rtc_ms_ticks(100)){;}


	esp8266_buff_write_enabled = 0;
	esp8266_buff_receive_complete = 1;
	//*esp8266_rx_buff_write_p = '\0';
	dataout[esp8266_rx_buff_charcount] = '\0';
}

ISR(USARTE0_RXC_vect){
	
	if(esp8266_buff_write_enabled){
		*(esp8266_rx_buff_write_p++) = USART_GetChar(&USARTE0);
		esp8266_rx_timesnap = rtc_tick;
		esp8266_rx_buff_charcount++;
	}
	else{
		USART_GetChar(&USARTE0);
	}
	
	
	
	
	
	//esp8266_rx_timesnap = rtc_tick;
	
	
}
//////////////////////////////////////////////////////////////////////////
typedef enum esp8266_wifi_uart_parity_enum{
	ESP8266_UART_PARITY_NONE ,//connect device to a wifi router
	ESP8266_UART_PARITY_ODD, //device is acting as an accesspoing
	ESP8266_UART_PARITY_EVEN,//combination of station nd softap
} esp8266_wifi_uart_parity_t;

typedef enum esp8266_wifi_uart_stopbits_enum{
	ESP8266_UART_STOPBITS_1 ,//connect device to a wifi router
	ESP8266_UART_STOPBITS_1_5, //device is acting as an accesspoing
	ESP8266_UART_STOPBITS_2,//combination of station nd softap
} esp8266_wifi_uart_stopbits_t;

void esp8266_uart_setup(int id, int baud, int databits, esp8266_wifi_uart_parity_t parity, esp8266_wifi_uart_stopbits_t stopbits, bool echo ){
	char cmd[50];
	char data[50];
	char paritystr[20];
	char stopbitstr[20];
	
	switch(parity){
		case ESP8266_UART_PARITY_NONE : strcpy(paritystr,"NONE");break;
		case ESP8266_UART_PARITY_ODD : strcpy(paritystr,"ODD");break;
		case ESP8266_UART_PARITY_EVEN : strcpy(paritystr,"EVEN");break;
		default: strcpy(paritystr,"NONE");break;
	}
	switch(stopbits){
		case ESP8266_UART_STOPBITS_1 : strcpy(stopbitstr,"1");break;
		case ESP8266_UART_STOPBITS_1_5 : strcpy(stopbitstr,"1_5");break;
		case ESP8266_UART_STOPBITS_2 : strcpy(stopbitstr,"2");break;
		default: strcpy(stopbitstr,"1");break;
	}
	
	sprintf(cmd,"uart.setup(%d, %d, %d, %s, %s, %d)",id,baud,databits,paritystr,stopbitstr,echo);
	//esp8266_send(cmd);
 	esp8266_send_receive(cmd,data);
// 	esp8266_rx_buff_receive_timeout(500,data);
}

//////////////////////////////////////////////////////////////////////////
typedef enum esp8266_wifi_mode_enum{
	ESP8266_WIFI_MODE_STATION = 1 ,//connect device to a wifi router
	ESP8266_WIFI_MODE_SOFTAP = 2, //device is acting as an accesspoing
	ESP8266_WIFI_MODE_STATIONAP = 3,//combination of station nd softap
	ESP8266_WIFI_MODE_NULLMODE = 0,//wifi is off
} esp8266_wifi_mode_t;


typedef enum esp8266_wifi_phymode_enum{
	ESP8266_WIFI_PHYMODE_B,
	ESP8266_WIFI_PHYMODE_G,
	ESP8266_WIFI_PHYMODE_N,
} esp8266_wifi_phymode_t;

typedef enum esp8266_wifi_sta_status_enum{
	ESP8266_WIFI_STA_STATUS_IDLE,
	ESP8266_WIFI_STA_STATUS_CONNECTING,
	ESP8266_WIFI_STA_STATUS_WRONG_PASSWORD,
	ESP8266_WIFI_STA_STATUS_NO_AP_FOUND,
	ESP8266_WIFI_STA_STATUS_CONNECT_FAIL,
	ESP8266_WIFI_STA_STATUS_GOT_IP,
} esp8266_wifi_sta_status_t;

typedef enum esp8266_wifi_tls_rsa_enum{
	TLS_RSA_WITH_AES_128_CBC_SHA,
	TLS_RSA_WITH_AES_256_CBC_SHA,
	TLS_RSA_WITH_RC4_128_SHA,
	TLS_RSA_WITH_RC4_128_MD5,
} esp8266_wifi_tls_rsa_t;

typedef enum esp8266_AP_auth_mode_enum{
	ESP8266_ECN_OPEN,
	ESP8266_ECN_WEP,
	ESP8266_ECN_WPA_PSK,
	ESP8266_ECN_WPA2_PSK,
	ESP8266_ECN_WPA_WPA2_PSK,
}esp8266_AP_auth_mode_t;

typedef struct esp8266_available_AP_enum{
	char BSSID[20];//mac address
	char SSID[32];	
	int RSSI;//signal strength
	esp8266_AP_auth_mode_t auth_mode;
	int channel;
} esp8266_available_AP_t;

void esp8266_setmode(esp8266_wifi_mode_t mode){
	char modestr[10];
	char cmd[40];
	char data[50];

	if(mode != ESP8266_WIFI_MODE_NULLMODE){
		switch(mode){
			case ESP8266_WIFI_MODE_STATION : sprintf(modestr,"STATION");break;
			case ESP8266_WIFI_MODE_SOFTAP : sprintf(modestr,"SOFTAP");break;
			case ESP8266_WIFI_MODE_STATIONAP : sprintf(modestr,"STATIONAP");break;
			//case ESP8266_WIFI_MODE_NULLMODE : sprintf(modestr,"NULLMODE");break;
			default:break;
		}
		
		sprintf(cmd,"wifi.setmode(wifi.%s)",modestr);
		esp8266_send_receive_timeout(cmd,data,2000);
		
		if(esp8266_getmode() == mode){
			printf("walla you are doing great\n");
		}
		else{
			printf("something is really screwed\n");
		}
	}


		
// 	
}

int esp8266_getmode(){

	char cmd[50];
	char data[50];
	sprintf(cmd,"print(wifi.getmode())");
	//esp8266_send(cmd);
	//esp8266_send_receive(cmd,NULL);
	//esp8266_rx_buff_receive_timeout(500,data);


	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
	

	int myargc;
	char *cmd_args[10];
	tokenize(data,&myargc,"\n", 10, cmd_args);
// 
// 	for(int i=0;i<myargc;i++){
// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	}

	return atoi(cmd_args[1]);
}


void esp8266_sta_config(char *ssid, char *password, bool autoconn, char *bssid){

	char cmd[40];
	char data[50];
	if(autoconn == '\0'){
		sprintf(cmd,"wifi.sta.config(\"%s\",\"%s\")",ssid,password);
	}
	else if(autoconn == 1){
		sprintf(cmd,"wifi.sta.config(\"%s\",\"%s\",%d,\"%s\" )",ssid,password,autoconn,bssid);
	}
	
	
// 	esp8266_send_receive(cmd,data);
// 	esp8266_rx_buff_receive_timeout(500,data);

	esp8266_send_receive_timeout(cmd,data,3000);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);


// 	//ESP8266_Wait_ms(5000);//stabilize
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(data,&myargc,"\n", 10, cmd_args);
// 
// 	for(int i=0;i<myargc;i++){
// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	}
		
	
}

int esp8266_sta_getap(rtctick_t scan_time,esp8266_available_AP_t *ap_data){
	//ESP8266_Wait_ms(2000);//stabilize
	if (scan_time < 5000) scan_time = 5000;
	
	char cmd[100];
	char data[1000];
	sprintf(cmd,"function listap(t) for k,v in pairs(t) do  print(k..\" # \"..v) end end wifi.sta.getap(1, listap)");
	//esp8266_send_receive(cmd,data);
	//printf("%s",data);
	
	esp8266_send_receive_timeout(cmd,data,scan_time);
	//esp8266_rx_buff_receive_timeout(scan_time,data);
/*	printf("%s",data);*/
	
	int myargc;
	char *cmd_args[20];
	tokenize(data,&myargc,">\r\n", 20, cmd_args);


	for(int i=1;i<myargc;i++){
		int myargc2;
		char *cmd_args2[20];
		tokenize(cmd_args[i],&myargc2,"#(),\"",10,cmd_args2);
		
		strcpy(ap_data[i-1].BSSID,cmd_args2[0]);
		strcpy(ap_data[i-1].SSID,cmd_args2[1]);
		ap_data[i-1].RSSI = atoi(cmd_args2[2]);
		ap_data[i-1].auth_mode = atoi(cmd_args2[3]);
		ap_data[i-1].channel = atoi(cmd_args2[4]);
	}
	
	strcpy(ap_data[0].BSSID,ap_data[0].BSSID + 1);//this is to remove the initial white space for the first available bssid
	
	return myargc-1;
		//ESP8266_Wait_ms(2000);//stabilize
}



void esp8266_sta_connect(){
	char data[100];
	char cmd[40];
	sprintf(cmd,"wifi.sta.connect()");
	//esp8266_send_receive_2(cmd,data);
	esp8266_send_receive_timeout(cmd,data,500);
	//esp8266_rx_buff_receive_timeout(500,data);
	
}

void esp8266_sta_disconnect(){
	char data[100];
	char cmd[40];
	sprintf(cmd,"wifi.sta.disconnect()");
	esp8266_send_receive_timeout(cmd,data,500);
	//esp8266_rx_buff_receive_timeout(500,data);
	
}

void esp8266_sta_getip(){
	char data[50];
	char cmd[60];

	
	sprintf(cmd,"print(wifi.sta.getip())");
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
	
}

void esp8266_sta_getmac(){
	char data[50];
	char cmd[30];

	sprintf(cmd,"print(wifi.sta.getmac())");
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
	
	
	
// 	
// 	int myargc;
// 		char *cmd_args[10];
// 		tokenize(data,&myargc," >\t\r\n", 10, cmd_args);
// 	
// 		// 	for(int i=0;i<myargc;i++){
// 		// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 		// 	}
// 	
// 	printf("mac address : %s\n",cmd_args[1]);
	
	
// 	
// 	sprintf(cmd,"print(wifi.sta.getmac())");
// 	//printf("%s",cmd);
// 	esp8266_send_receive(cmd,data);
// 	//esp8266_rx_buff_receive_timeout(200,data);
// 	printf("\n%s",data);
}

void esp8266_sta_get_status(){

	char data[50];
	char cmd[50];

	sprintf(cmd,"print(wifi.sta.status())");
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(500,data);

	printf("\n%s",data);
	
	int myargc;
	char *cmd_args[10];
	tokenize(data,&myargc," >\t\r\n", 10, cmd_args);
	
	// 	for(int i=0;i<myargc;i++){
	// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
	// 	}
	
	
}
//////////////////////////////////////////////////////////////////////////ESP8266_nodemcu_net
typedef enum esp8266_net_constants_enum{
	ESP8266_NET_TCP,
	ESP8266_NET_UDP,
}esp8266_net_constants_t;

void esp8288_net_create_connection(esp8266_net_constants_t type, bool secure){
	
}


//////////////////////////////////////////////////////////////////////////ESP8266_nodemcu_mqtt


void esp8266_mqtt_create_client(char *mqttclient, char *clientid, int keepalive, char *user, char *password){
		char data[100];
		char cmd[500];

		sprintf(cmd,"%s = mqtt.Client(\"%s\", %d, \"%s\", \"%s\")",mqttclient,clientid,keepalive,user,password);
		//printf("%s",cmd);
		esp8266_send_receive(cmd,data);
		//esp8266_rx_buff_receive_timeout(200,data);
		printf("\n%s",data);
}

void esp8266_mqtt_client_connect(char *mqttclient, char *host, int port,esp8266_wifi_tls_rsa_t security, char *function){
	char data[100];
	char cmd[500];

	sprintf(cmd,"%s:connect(\"%s\", \"%d\", %d ,1, %s)",mqttclient,host,port,0,function);
	//printf("%s",cmd);
	//esp8266_send_receive_2(cmd,data);
	esp8266_send_receive_timeout(cmd,data,5000);
	//esp8266_rx_buff_receive_timeout(2000,data);
	printf("\n%s",data);
	
}

void esp8266_mqtt_client_close(char *mqttclient){
	char data[100];
	char cmd[50];

	sprintf(cmd,"%s:close()",mqttclient);
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(2000,data);
	printf("\n%s",data);
	
}

void esp8266_mqtt_client_lwt(char *mqttclient, char *topic, char *message, int qos, bool retain_flag){
	char data[100];
	char cmd[500];

	sprintf(cmd,"%s:lwt(\"%s\",\"%s\",%d,%d)", mqttclient,topic,message,qos,retain_flag);
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
}


void esp8266_mqtt_client_publish(char *mqttclient, char *topic, char *message, int qos, bool retain_flag, char *function){
	char data[100];
	char cmd[500];

	sprintf(cmd,"%s:publish(\"%s\",\"%s\",%d,%d, %s)", mqttclient,topic,message,qos,retain_flag,function);
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
}

void esp8266_mqtt_client_subscribe(char *mqttclient, char *topic, int qos, char *function){
	char data[100];
	char cmd[500];

	sprintf(cmd,"%s:subscribe(\"%s\",%d, %s)", mqttclient,topic,qos,function);
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
}

void esp8266_mqtt_client_callback_on_message(char *mqttclient, char *function){
	char data[100];
	char cmd[500];

	sprintf(cmd,"%s:on(\"message\", %s)", mqttclient,function);
	//printf("%s",cmd);
	esp8266_send_receive(cmd,data);
	//esp8266_rx_buff_receive_timeout(200,data);
	printf("\n%s",data);
}


#endif /* ESP8266NODEMCU_H_ */