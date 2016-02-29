// /*
//  * esp8266.h
//  *
//  * Created: 29-Dec-15 2:01:12 AM
//  *  Author: emon1
//  */ 
// 
// 
// #ifndef ESP8266_H_
// #define ESP8266_H_
// 
// 
// #ifdef __cplusplus
// extern "C" {
// #endif
// 
// 
// #define ESP8266_USE_ATCMD_NODEMCU 1 //0 -> AT+COMMAND, 1-> NODEMCU
// 
// 
// 
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <string.h>
// 
// FILE esp8266_stream;
// 
// typedef enum esp8266_wifi_mode_enum{
// 	ESP8266_WIFI_MODE_CLIENT = 1,
// 	ESP8266_WIFI_MODE_HOST = 2, 
// 	ESP8266_WIFI_MODE_CLIENT_HOST = 3, 
// } esp8266_wifi_mode_t;
// 
// typedef enum esp8266_ecn{
// 	ESP8266_ECN_OPEN,
// 	ESP8266_ECN_WEP,
// 	ESP8266_ECN_WPA_PSK,
// 	ESP8266_ECN_WPA2_PSK,
// 	ESP8266_ECN_WPA_WPA2_PSK,
// }esp8266_ecn_t;
// 
// 
// typedef enum esp8266_connection_type{
// 	TCP,
// 	UDP
// }esp8266_connection_type_t;
// 
// typedef struct ESP8266_available_AP{
// 	esp8266_ecn_t ecn;  
// 	char ssid[32];
// 	int sig_strength;
// 	char mac[20];
// } ESP8266_available_AP_t;
// 
// 
// typedef struct ESP8266_cpnnected_AP{
// 	uint8_t id;
// 	esp8266_connection_type_t type;
// 	char * IP_addr;
// 	int port;
// 	uint8_t tetype;
// } ESP8266_cpnnected_AP_t;
// 
// 
// 
// 
// char esp8266_rxc_buff[400];
// volatile char *esp8266_rxc_buff_p = esp8266_rxc_buff;
// 
// volatile rtctick_t esp8266_timesnap = 0;
// volatile rtctick_t esp8266_wait_timesnap = 0;
// 
// 
// static void tokenize(char * command, int * arg_count, const char *delim, int max_tokens, char *arg_strings[]){

// 	int arg_index = 0;
// 
// 	char *pch;
// 	pch = strtok(command,delim);
// 	while(pch != NULL){
// 		arg_strings[arg_index++] = pch;
// 		//arg_index++;
// 		if(arg_index >max_tokens)break;
// 		pch = strtok(NULL,delim);
// 	}
// 	*arg_count = arg_index;
// }
// 
// 
// void esp8266_init(){
// 	usart_config(&USARTE0,&PORTE,USART_CMODE_ASYNCHRONOUS_gc,USART_PMODE_DISABLED_gc,USART_CHSIZE_8BIT_gc,false,false);
// 	usart_baud(&USARTE0,9600,-3);
// 	usart_set_interrupt_level(&USARTE0, USART_RXCINTLVL_HI_gc, USART_TXCINTLVL_OFF_gc, USART_DREINTLVL_OFF_gc);
// 	create_file_stream(&esp8266_stream,&USARTE0_putchar,&USARTE0_getchar);
// }
// 
// 
// void esp8266_receive(int ms_wait){
// 	esp8266_timesnap = rtc_tick;
// 	esp8266_wait_timesnap = rtc_tick;
// 	//char strout[200];
// 	//char *strout_p;
// 	
// 	while( (rtc_tick-esp8266_wait_timesnap<rtc_ms_ticks(ms_wait))){
// 		while( (rtc_tick-esp8266_timesnap<rtc_ms_ticks(200)));
// 	}
// 	*esp8266_rxc_buff_p = '\0';
// 	esp8266_rxc_buff_p = esp8266_rxc_buff;
// 
// }
// 
// void esp8266_wait_receive(){
// 	esp8266_timesnap = rtc_tick;
// 	esp8266_wait_timesnap = rtc_tick;
// 
// 	while( (rtc_tick-esp8266_timesnap<rtc_ms_ticks(50)));
// 	*esp8266_rxc_buff_p = '\0';
// 	printf("%s",esp8266_rxc_buff);
// 	
// 	char str[10];
// 	char id[10];
// 	char len[10];
// 	char data_str[100];
// 
// 	if(esp8266_rxc_buff_p>esp8266_rxc_buff){
// 			//sscanf (sentence,"%s %*s %s %*s",str,str2);
// 			sscanf (esp8266_rxc_buff,"\r\r\n+%9[^,],%9[^,],%9[^:]:%s[^\n]\n",str,id,len,data_str);
// 			printf("%s",str);
// 			
// 			
// 			if(!strcmp(str,"IPD\0")){
// 				int pre_data_len = 6 + strlen(str) + strlen(id) + strlen(len);
// 				int data_len = atoi(len);
// 				char data[data_len];
// 
// 				printf("THE DISPLACEMENT IS : %d\n", pre_data_len);
// 
// 				//data_p = data_str;
// 				for(int i=0;i<data_len;i++){
// 					data[i] = esp8266_rxc_buff[pre_data_len+i];
// 					printf(".%c\n",data[i]);
// 				}
// 				data[data_len] = '\0';
// 
// 				printf ("\n%s\n%s\n%s\n%s\n",str,id,len,data_str);
// 							
// 				ssd1306_clear();
// 				ssd1306_set_page_address(0);
// 				ssd1306_set_column_address(0);
// 				ssd1306_write_text(data);
// 			}
// 	}
// 
// 	
// 	esp8266_rxc_buff_p = esp8266_rxc_buff;
// 	
// 	
// }
// 
// 
// #if  (ESP8266_USE_ATCMD_NODEMCU == 0)
// 
// 
// int esp8266_test(){
// 	printf("testing wifi\n");
// 	fprintf(&esp8266_stream,"AT\r\n");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
//	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," \n\r", 10, cmd_args);
// 	
// 	if(!strcmp("OK",cmd_args[1])){
// 		printf("TEST OK\n");
// 		return 1;
// 	}
// 	else {
// 		printf("TEST FAIL\n");
// 		return -1;
// 	}
// }
// 
// int esp8266_reset(){
// 	printf("resetting wifi\n");
// 	fprintf(&esp8266_stream,"AT+RST\n\r");
// 	esp8266_receive(8000);
// 	//printf("%s",esp8266_rxc_buff);
// 	printf("THE STRING LENGTH IS  ::::::::::::::: %d\n",strlen(esp8266_rxc_buff));
// 	
// 	int myargc;
//	char *cmd_args[25];
// 	tokenize(esp8266_rxc_buff,&myargc,"\n\r", 25, cmd_args);
// 	
// 	for(int i=0;i<myargc;i++){
// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	}
// 	
// 	if(!strcmp("ready",cmd_args[20])){
// 		printf("RESET COMPLETE\n");
// 		return 1;
// 	}
// 	else {
// 		printf("RESET ERROR\n");
// 		return -1;
// 	}
// 	
// }
// 
// void esp8266_version(){
// 	printf("getting ESP8266 version\n");
// 	fprintf(&esp8266_stream,"AT+GMR\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	int myargc;
//	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," -,\n\r", 10, cmd_args);
// 	
// 	//for(int i=0;i<myargc;i++){
// 	//printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	//}
// 	
// 	printf("the version : %s\n",cmd_args[1]);
// 	
// 
// }
// 
// 
// int esp8266_wifi_mode(esp8266_wifi_mode_t mode){
// 	printf("setting wifi mode (CWMODE)\n");
// 	fprintf(&esp8266_stream,"AT+CWMODE=%d\n\r",mode);
// 	esp8266_receive(2000);
// 
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r\"", 10, cmd_args);
// 
// 	if(!strcmp("OK",cmd_args[2])){
// 		printf("WIFI MODE CONFIGURED\n");
// 		return 1;
// 	}
// 	else {
// 		printf("FAILED\n");
// 		return -1;
// 	}
// }
// 
// esp8266_wifi_mode_t esp8266_check_wifi_mode(){
// 	printf("checking wifi mode (CWMODE)\n");
// 	fprintf(&esp8266_stream,"AT+CWMODE?\n\r");
// 	esp8266_receive(2000);
// 
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc,":\n\r", 10, cmd_args);
// 	
// 	if(!strcmp("OK",cmd_args[3])){
// 		printf("WIFI MODE = %s\n", cmd_args[2]);
// 		return (esp8266_wifi_mode_t) atoi(cmd_args[2]);
// 	}
// 	else {
// 		printf("ERROR\n");
// 		return -1;
// 	}
// }
// 
// 
// int esp8266_list_access_points(ESP8266_available_AP_t *ap_data){
// 	printf("Listing Available AP\n");
// 	fprintf(&esp8266_stream,"AT+CWLAP\n\r");
// 	esp8266_receive(8000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 
// 	int myargc;
//	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," +\n\r", 10, cmd_args);
// 	
// 	//TEST/////////////////////////////////////////// /////////////////////////////////////////////
// 	int OK_count = 0;
// 	for(int i=0;i<myargc;i++){
// 		//printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 		if(!strcmp(cmd_args[i],"OK")){
// 			OK_count = i;
// 		}
// 	}
// 	
// 	int i=0;
// 	for(i=0+2;i<OK_count;i++){
// 		int myargc2;
// 		char *cmd_args2[10];
// 		tokenize(cmd_args[i],&myargc2,"(),\"",10,cmd_args2);
// 
// 		ap_data[i-2].ecn = atoi(cmd_args2[1]);
// 		strcpy(ap_data[i-2].ssid,cmd_args2[2]);
// 		ap_data[i-2].sig_strength = atoi(cmd_args2[3]);
// 		strcpy(ap_data[i-2].mac,cmd_args2[4]);
// 
// 	}
// 	
// 	//ap_data=available_AP;
// 	return i-2;
// 
// }
// 
// 
// int esp8266_connect_access_points(char *SSID, char *password){
// 	printf("Joining Available AP\n");
// 	fprintf(&esp8266_stream,"AT+CWJAP=\"%s\",\"%s\"\n\r",SSID,password);
// 	esp8266_receive(10000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 	printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	if(!strcmp("OK",cmd_args[3])){
// 		printf("successfully connected to %s\n",cmd_args[1]);
// 		return 1;
// 	}
// 	else {
// 		printf("connection failed\n");
// 		return -1;
// 	}
// 	
// 	
// 	
// 	
// }
// 
// int esp8266_check_connected_access_points(char *ssid){
// 	printf("checking connected AP\n");
// 	fprintf(&esp8266_stream,"AT+CWJAP?\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc,"\n\r\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 	printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	if(!strcmp("OK",cmd_args[3])){
// 		strcpy(ssid,cmd_args[2]);
// 		return 1;
// 	}
// 	else {
// 		printf("ERROR\n");
// 		return -1;
// 	}
// 	
// 	
// 	
// }
// 
// int esp8266_disconnect_access_points(){
// 	printf("Quitting  AP\n");
// 	fprintf(&esp8266_stream,"AT+CWQAP\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc,"\n\r\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 	printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	if(!strcmp("OK",cmd_args[1])){
// 		printf("DISCONNECTION SUCCESFUL\n");
// 		return 1;
// 	}
// 	else {
// 		printf("ERROR\n");
// 		return -1;
// 	}
// }
// 
// 
// 
// 
// 
// 
// 
// void esp8266_get_IP(char *IPaddr){
// 	printf("Getting IP Address\n");
// 	fprintf(&esp8266_stream,"AT+CIFSR\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	//printf("the IP address is : %s\n", cmd_args[2]);
// 	strcpy(IPaddr,cmd_args[2]);
// 	
// }
// 
// 
// 
// void esp8266_get_MAC(char *MAC){
// 	printf("Getting MAC Address\n");
// 	fprintf(&esp8266_stream,"AT+CIFSR\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 
// 	//printf("the MAC address is : %s\n", cmd_args[4]);
// 	strcpy(MAC,cmd_args[4]);
// }
// 
// 
// 
// 
// 
// 
// 
// int esp8266_multiple_connection_enable(uint8_t en){
// 	printf("setting CIPMUX=%d\n",en);
// 	fprintf(&esp8266_stream,"AT+CIPMUX=%d\n\r",en);
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	
// 	if(!strcmp("OK",cmd_args[2])){
// 		printf("successfully enabled multiple connection\n");
// 		return 1;
// 	}
// 	else{
// 		printf("error enabling multiple connection\n");
// 		return -1;
// 	}
// 	
// 	
// }
// 
// 
// int esp8266_check_multiple_connection(){
// 	printf("checking CIPMUX\n");
// 	fprintf(&esp8266_stream,"AT+CIPMUX?\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r:\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	if(!strcmp("OK",cmd_args[3])){
// 		if(atoi(cmd_args[2])){
// 			printf("multiple connection enabled\n");
// 			return 1;
// 		}
// 		else{
// 			printf("multiple connection disabled\n");
// 			return 0;
// 		}
// 	}
// 	else {
// 		printf("connection failed\n");
// 		return -1;
// 	}
// }
// 
// // void esp8266_check_join_devices_IP(){
// // 	printf("checking devices IP\n");
// // 	fprintf(&esp8266_stream,"AT+CWLIF\n\r");
// // 	esp8266_receive(2000);
// // 	printf("%s",esp8266_rxc_buff);
// // }
// 
// int esp8266_check_connection_status(ESP8266_cpnnected_AP_t * device){
// 	printf("checking connection status\n");
// 	fprintf(&esp8266_stream,"AT+CIPSTATUS\n\r");
// 	esp8266_receive(500);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	int myargc2;
// 	char *cmd_args[10];
// 	char *cmd_args2[10];
// 	tokenize(esp8266_rxc_buff,&myargc,"\n\r", 10, cmd_args);
// 	
// 	
// 	//printf("THE ARGUMENT COUNT IS : %d\n",myargc);
// 	
// 	
// 	int OK_count = 0;
// 	for(int i=0;i<myargc;i++){
// 		//printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 		if(!strcmp(cmd_args[i],"OK")){
// 			OK_count = i;
// 			break;
// 		}
// 	}
// 	
// 	
// 	//printf("THE OK COUNT IS TOKEN NO : %d\n",OK_count);
// 	
// 	
// 	tokenize(cmd_args[1],&myargc2,":", 10, cmd_args2);
// 	//printf("THE STATUS IS : %s\n",cmd_args2[1]);
// 	int status = atoi(cmd_args2[1]);
// 	
// 	for(int i=2;i<OK_count;i++){
// 		int myargc3;
// 		char *cmd_args3[10];
// 		tokenize(cmd_args[i],&myargc3,"+\r\n",10,cmd_args3);
// 		
// 		for(int j=0;j<myargc3;j++){
// 			printf("\tconnected device --------->>>> %s\n",cmd_args3[j]);
// 			///WRITE CODE TO FORMAT THE CONNECETED DEVICE INFO
// 		}
// 	}
// 	
// 	return status;
// 	
// 	
// }
// 
// void esp8266_set_server(int port){
// 	printf("setting server\n");
// 	fprintf(&esp8266_stream,"AT+CIPSERVER=1,%d\n\r",port);
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r:\"", 10, cmd_args);
// 	
// 	// 	for(int i=0;i<myargc;i++){
// 	// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	// 	}
// 	
// 	
// 	if(!strcmp(cmd_args[3],"OK")){
// 		printf("server started successfully\n");
// 	}
// 	else if( (!strcmp(cmd_args[3],"no")) && (!strcmp(cmd_args[4],"change")) ){
// 		printf("server already running\n");
// 	}
// }
// 
// void esp8266_close_server(){
// 	printf("closing server\n");
// 	fprintf(&esp8266_stream,"AT+CIPSERVER=0\n\r");
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r:\"", 10, cmd_args);
// 	
// 	for(int i=0;i<myargc;i++){
// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	}
// }
// 
// 
// 
// 
// 
// void esp8266_start_connection(uint8_t id, esp8266_connection_type_t type, char * addr, int port){
// 	
// 	printf("starting connection\n");
// 	char type_str[3];
// 	if(type==TCP){
// 		strcpy(type_str,"\"TCP\"");
// 	}
// 	else if(type == UDP){
// 		strcpy(type_str,"\"UDP\"");
// 	}
// 	
// 
// 	if(esp8266_check_multiple_connection()){
// 		fprintf(&esp8266_stream,"AT+CIPSTART=%d,%s,\"%s\",%d\n\r",id,type_str,addr,port);
// 	}
// 	else {
// 		fprintf(&esp8266_stream,"AT+CIPSTART=%s,\"%s\",%d\n\r",type_str,addr,port);
// 	}
// 	
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// 
// 
// }
// 
// void esp8266_close_connection(uint8_t id){
// 	printf("closing connection\n");
// 	fprintf(&esp8266_stream,"AT+CIPCLOSE=%d\n\r",id);
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// }
// 
// void esp8266_close_all_connection(){
// 	printf("closing connection\n");
// 	fprintf(&esp8266_stream,"AT+CIPCLOSE\n\r");
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// }
// 
// void esp8266_send_data_to_id(uint8_t id, int len, uint8_t *data){
// 	//printf("sending data\n");
// 	fprintf(&esp8266_stream,"AT+CIPSEND=%d,%d\n\r",id,len+1);
// 	esp8266_receive(100);
// 	//fprintf(&esp8266_stream,"%s\n\r",data);
// 	
// 	for(int i=0;i<=len;i++){
// 		fputc(data[i],&esp8266_stream);
// 	}
// 	
// 	
// 	esp8266_receive(100);
// 	
// 	//printf("%s",esp8266_rxc_buff);
// }
// 
// 
// 
// 
// 
// 
// void esp8266_server_timeout(int time_sec){
// 	printf("setting server timeout\n");
// 	fprintf(&esp8266_stream,"AT+CIPSTO=%d\n\r",time_sec);
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// 	
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r:\"", 10, cmd_args);
// 	
// 	for(int i=0;i<myargc;i++){
// 		printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	}
// 	
// }
// 
// int esp8266_check_server_timeout(){
// 	printf("checking server timeout\n");
// 	fprintf(&esp8266_stream,"AT+CIPSTO?\n\r");
// 	esp8266_receive(2000);
// 	//printf("%s",esp8266_rxc_buff);
// 	
// 	int myargc;
// 	char *cmd_args[10];
// 	tokenize(esp8266_rxc_buff,&myargc," =-,\n\r:\"", 10, cmd_args);
// 	
// 	//for(int i=0;i<myargc;i++){
// 	//printf("the token : %d is -> %s\n",i,cmd_args[i]);
// 	//}
// 	
// 	
// 	if(!strcmp(cmd_args[3],"OK")){
// 		printf("server timeout is : %d\n",atoi(cmd_args[2]));
// 		return atoi(cmd_args[2]);
// 	}
// 	else{
// 		return -1;
// 	}
// }
// 
// void esp8266_set_connection_single(uint8_t type, char *IPaddr, int port){
// 	printf("creating connection\n");
// 	if(type == 0) fprintf(&esp8266_stream,"AT+CIPSTART=\"TCP\",%s,%d\n\r",IPaddr,port);
// 	else if(type == 1) fprintf(&esp8266_stream,"AT+CIPSTART=\"UDP\",%s,%d\n\r",IPaddr,port);
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// }
// 
// void esp8266_send_data_multiple(uint8_t id, char *data){
// 	// 	printf("the size of the data is : %d\n",strlen(data));
// 	// 	fprintf(&esp8266_stream,"AT+CIPSEND=%d,%d\r\n",id,strlen(data)+6);//6 is fine
// 	// 	esp8266_receive(1000);
// 	// 	printf("%s",esp8266_rxc_buff);
// 	// 	fprintf(&esp8266_stream,"%s",data);
// 	// 	esp8266_receive(3000);
// 	// 	fprintf(&esp8266_stream,"\r\n\r\n");
// 	// 	printf("%s",esp8266_rxc_buff);
// 
// 	printf("the size of the data is : %d\n",strlen(data));
// 	fprintf(&esp8266_stream,"AT+CIPSEND=%d,%d\r\n",id,strlen(data)+6);//6 is fine
// 	esp8266_receive(1000);
// 	printf("%s",esp8266_rxc_buff);
// 	rtc_ms_delay(500);
// 
// 	fprintf(&esp8266_stream,"%s",data);
// 	//fprintf(&esp8266_stream,"\r\n");
// 	//rtc_ms_delay(500);
// 	esp8266_receive(500);
// 	
// 	printf("%s",esp8266_rxc_buff);
// 	fprintf(&esp8266_stream,"\r\n\r\n");
// 	esp8266_receive(2000);
// 	//fprintf(&esp8266_stream,"\r\n\r\n");
// 	printf("%s",esp8266_rxc_buff);
// 
// }
// 
// 
// void esp8266_set_connection_multiple(uint8_t id, uint8_t type, char *IPaddr, int port){
// 	printf("creating connection\n");
// 	if(type == 0) fprintf(&esp8266_stream,"AT+CIPSTART=%d,TCP,%s,%d\n\r",id,IPaddr,port);
// 	else if(type == 1) fprintf(&esp8266_stream,"AT+CIPSTART=%d,UDP,%s,%d\n\r",id,IPaddr,port);
// 	esp8266_receive(2000);
// 	printf("%s",esp8266_rxc_buff);
// }
// 
// 
// #elif (ESP8266_USE_ATCMD_NODEMCU == 1)
// 
// 
// 
// 
// void esp8266_nodemcu_send_cmd(char *cmd){
// 	fprintf(&esp8266_stream,"%s\r\n",cmd);
// }
// 
// 
// void esp8266_nodemcu_gpio_mode(int pin, int gpio_mode){
// 
// 	if(gpio_mode){
// 		fprintf(&esp8266_stream,"gpio.mode(%d,gpio.OUTPUT)\r\n",pin);
// 	}
// 	else{
// 		fprintf(&esp8266_stream,"gpio.mode(%d,gpio.INPUT)\r\n",pin);
// 	}
// }
// 
// 
// 
// 
// 
// 
// void esp8266_nodemcu_gpio_write(int pin, int val){
// 	if(!val){
// 		//fprintf(&esp8266_stream,"gpio.write(%d,gpio.LOW)\r\n",pin);
// 	}
// 	else{
// 		//fprintf(&esp8266_stream,"gpio.write(%d,gpio.HIGH)\r\n)",pin);
// 	}
// 	//fprintf(&esp8266_stream,"gpio.write(%d,gpio.HIGH)\r\n",pin);
// }
// 
// 
// 
// #endif
// 
// 
// 
// 
// 
// 
// ISR(USARTE0_RXC_vect){
// 
//  	*(esp8266_rxc_buff_p++) = USARTE0.DATA;
//  	esp8266_timesnap = rtc_tick;
// 
// }
// 
// 
// #ifdef __cplusplus
// }
// #endif
// 
// 
// 
// 
// #endif /* ESP8266_H_ */