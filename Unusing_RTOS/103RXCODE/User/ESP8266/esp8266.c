#include "esp8266.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

int WIFI_Mode_FLAG = 1;

void Wifi_Init(void)
{
	if(WIFI_Mode_FLAG == 0)
	{
		u2_printf("AT+CWMODE=3\r\n");//设置为 AP模式
	
		Delay_ms(500);
		
		u2_printf("AT+RST\r\n");//重启
		Delay_ms(1500);
		
		u2_printf("AT+CIPAP=\"192.168.x.x\"\r\n");//设置IP:192.168.x.x
		Delay_ms(500);

		u2_printf("AT+CWSAP=\"FHQ\",\"12345678\",5,0\r\n");//设置wifi名称是CZJ，密码12345678，最多5个人同时连接，连接时无需密码；
		Delay_ms(500);
		
		u2_printf("AT+CIPMUX=1\r\n");//启动多连接
		Delay_ms(500);
		
		u2_printf("AT+CIPSERVER=1,8080\r\n");//设置端口8080
		Delay_ms(500);

//		u2_printf("AT+CIPSEND=0,5\r\n");
		
//		AT+CIPSEND=0,5
//		hello,tcp
	}
	if(WIFI_Mode_FLAG == 1)
	{

		u2_printf("AT+CWMODE=1\r\n");//设置为 softAP+station 共存模式
		Delay_ms(500);
;
		u2_printf("AT+RST\r\n");//重启;
		Delay_ms(1500);

		u2_printf("AT+CWJAP=\"Mirror_zero\",\"11111111\"\r\n");//连接wifi
		Delay_ms(3000);

		u2_printf("AT+CIPMUX=0\r\n");//启动单连接
		Delay_ms(500);

		u2_printf("AT+CIPSTART=\"TCP\",\"xxx.xxx.xxx.xxx\",50021\r\n");//连接服务器，协议TCP，IP地址xxx.xxx.xxx.xxx，端口50021
		Delay_ms(3000);

		u2_printf("AT+CIPMODE=1\r\n");;
		Delay_ms(500);

		u2_printf("AT+CIPSEND\r\n");//开启透传
		Delay_ms(500);

	}

}






































/*
//定义全局变量
volatile uint32_t esp8266_transparent_transmission_sta=0;

//配置通信串口
void esp8266_init(void)
{
	uart2_init(115200); 	//串口初始化
}

// 配置WiFi连接热点 
int32_t Esp8266_Init(void)
{
	int32_t ret;
	
	// esp8266_wifi利用串口1通信,前期已配置串口1
	esp8266_init();

	Delay_ms(500);	
	
	// 退出透传模式，才能输入AT指令
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("001: esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	Delay_ms(500);	
	printf("001: esp8266_exit_transparent_transmission success\r\n");
	Delay_ms(1000);
	
	
	// 复位模块
	ret=esp8266_reset();
	if(ret)
	{
		printf("002: esp8266_reset fail\r\n");
		return -2;
	}
	Delay_ms(500);	
	printf("002: esp8266_reset success\r\n");
	Delay_ms(1000);

	
	// 关闭回显
	ret=esp8266_enable_echo(0);
	if(ret)
	{
		printf("003: esp8266_enable_echo(0) fail\r\n");
		return -3;
	}	
	Delay_ms(500);	
	printf("003: esp8266_enable_echo(0)success\r\n");
	Delay_ms(1000);

	// 连接热点
	ret = esp8266_connect_ap(WIFI_SSID,WIFI_PASSWORD);
	if(ret)
	{
		printf("004: esp8266_connect_ap fail\r\n");
		return -4;
	}	
	Delay_ms(500);	
	printf("004: esp8266_connect_ap success\r\n");
	Delay_ms(1000);

	return 0;
}
//发送AT指令
void esp8266_send_at(char *str)
{
	//清空接收缓冲区
	memset((void *)USART2_RX_BUF,0, sizeof USART2_REC_LEN);
	
	//清空接收计数值
	USART2_RX_STA = 0;	
	
	USART_sendstr(USART, str);	
}

// 查找接收数据包中的字符串 
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&USART2_RX_BUF;
	
	//等待串口接收完毕或超时退出，strstr()寻找相应字符串，如果未找到则返回 Null;
	while((strstr(src,dest)==NULL) && timeout) //while(找到了 ！=  NULL && timeout == 0),退出循环；
	{		
		Delay_ms(1);
		timeout--;
	}

	if(timeout) 
		return 0;	//查找到了相关数据
		                    
	return -1; 
}

// 检查ESP8266是否正常 
int32_t esp8266_check(void)
{
	esp8266_send_at(" AT\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}

//  复位 
int32_t esp8266_reset(void)
{
	esp8266_send_at(" AT+RST\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}


//  回显打开或关闭 
int32_t esp8266_enable_echo(uint32_t b)
{
	if(b)
		esp8266_send_at("ATE1\r\n"); 
	else
		esp8266_send_at("ATE0\r\n"); 
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;

	return 0;
}

//  退出透传模式 
int32_t esp8266_exit_transparent_transmission (void)
{
	esp8266_send_at ("+++");
	
	//退出透传模式，发送下一条AT指令要间隔1秒
	Delay_ms(1000); 
	
	//记录当前esp8266工作在非透传模式
	esp8266_transparent_transmission_sta = 0;

	return 0;
}

//  进入透传模式 
int32_t esp8266_entry_transparent_transmission(void)
{
	//进入透传模式
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	Delay_ms(1000);Delay_ms(1000);

	//开启发送状态
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -2;
	Delay_ms(1000);Delay_ms(1000);

	//记录当前esp8266工作在透传模式
	esp8266_transparent_transmission_sta = 1;
	return 0;
}

//  检测连接状态 
int32_t esp8266_check_connection_status(void)
{
	esp8266_send_at("AT+CIPSTATUS\r\n");
	
	if(esp8266_find_str_in_rx_packet("STATUS:3",10000))
		if(esp8266_find_str_in_rx_packet("OK",10000))
			return -1;

	return 0;
}


 * 功能：连接热点
 * 参数：
 *         ssid:热点名
 *         pwd:热点密码
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. WIFI名和密码不正确
 *         2. 路由器连接设备太多,未能给ESP8266分配IP
 
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
#if 0	
	//不建议使用以下sprintf，占用过多的栈
	char buf[128]={0};
	
	//设置为STATION模式	
	esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;
	esp8266_send_at("AT+CIPMUX=0\r\n");
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -2;
	sprintf(buf,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);
	esp8266_send_at(buf); 
	if(esp8266_find_str_in_rx_packet("OK",5000))
		if(esp8266_find_str_in_rx_packet("CONNECT",5000))
			return -2;
#else
	//设置为AP+Station模式	
	esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -1;

	esp8266_send_at("AT+CIPMUX=0\r\n");
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -2;

	//连接热点
	esp8266_send_at("AT+CWJAP_CUR="); 
	esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
	esp8266_send_at(",");	
	esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
	esp8266_send_at("\r\n");
	//连接热点，务必等待该条指令返回WIFI GOT IP，表示连接成功后，再发送下面的指令；
	while(esp8266_find_str_in_rx_packet("WIFI GOT IP",5000));
#endif
	return 0;
}


 * 功能：使用指定协议(TCP/UDP)连接到服务器
 * 参数：
 *         mode:协议类型 "TCP","UDP"
 *         ip:目标服务器IP
 *         port:目标是服务器端口号
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. 远程服务器IP和端口号有误
 *         2. 未连接AP
 *         3. 服务器端禁止添加(一般不会发生)
 
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{

#if 0	
	//使用MQTT传递的ip地址过长，不建议使用以下方法，否则导致栈溢出
	//AT+CIPSTART="TCP","a10tC4OAAPc.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883，该字符串占用内存过多了
	
	char buf[128]={0};
	
	//连接服务器
	sprintf((char*)buf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
	
	esp8266_send_at(buf);
#else
	char buf[16]={0};
	esp8266_send_at("AT+CIPSTART=");
	esp8266_send_at("\"");	esp8266_send_at(mode);	esp8266_send_at("\"");
	esp8266_send_at(",");
	esp8266_send_at("\"");	esp8266_send_at(ip);	esp8266_send_at("\"");	
	esp8266_send_at(",");
	sprintf(buf,"%d",port);
	esp8266_send_at(buf);	
	esp8266_send_at("\r\n");
#endif
	
	if(esp8266_find_str_in_rx_packet("CONNECT",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000))
			return -1;
	return 0;
}

//  断开服务器 
int32_t esp8266_disconnect_server(void)
{
	esp8266_send_at("AT+CIPCLOSE\r\n");
		
	if(esp8266_find_str_in_rx_packet("CLOSED",2000))
		if(esp8266_find_str_in_rx_packet("OK",2000))
			return -1;

	return 0;	
}

// 使能多链接 
int32_t esp8266_enable_multiple_id(uint32_t b)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPMUX=%d\r\n", b);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

//  创建服务器 
int32_t esp8266_create_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=1,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

///关闭服务器 
int32_t esp8266_close_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=0,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}



以下为简单框架设置Esp8266_WiFi模块为AP模式进行工作，即设置AP局域网

void Wifi_Init(void)
{
	USART_sendstr(USART,"AT+CWMODE=3\r\n");//设置为 softAP+station 共存模式
	Delay_ms(500);
	
	USART_sendstr(USART,"AT+RST\r\n");//重启
	Delay_ms(1500);
	
	USART_sendstr(USART,"AT+CIPAP=\"192.168.1.1\"\r\n");//设置IP:192.168.1.1
	Delay_ms(500);

	USART_sendstr(USART,"AT+CWSAP=\"FHQ\",\"12345678\",5,0\r\n");//设置wifi名称是CZJ，密码12345678，最多5个人同时连接，连接时无需密码；
	Delay_ms(500);
	
	USART_sendstr(USART,"AT+CIPMUX=1\r\n");//启动多连接
	Delay_ms(500);
	
	USART_sendstr(USART,"AT+CIPSERVER=1,8080\r\n");//设置端口8080
	Delay_ms(500);
}

//wifi模块发送语句---每次固定发送num个字节
void wifisend(char *buf,int num)
{
	//每次wifi模块发送数据的时候，都事先发送一个固定前缀
	char sendfront[32];												//定义前缀的缓冲区
	sprintf(sendfront,"AT+CIPSEND=0,%d\r\n",num);					//组合字符串
	
	USART_sendstr(USART,sendfront);
	Delay_ms(5);
	USART_sendstr(USART,buf);
}

//发送len长度的字符串
void USART_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len)
{ 
	while(Len--)
	{				                          			//判断是否到达字符串结束符
	    USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); 	//等待发送完
	}
}

//发送一个完整的字符串
void USART_sendstr(USART_TypeDef* USARTx, char *Data) // "shijienameda"
{ 
	//循环发送1个字节，直到准备发送的字节是'\0',也就是字符串末尾，停止发送
	while(*Data!=0)
	{				                        
		USART_SendData(USARTx, *Data);
		Data++;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}

*/
