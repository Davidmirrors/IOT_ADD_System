#include "esp8266.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

int WIFI_Mode_FLAG = 1;

void Wifi_Init(void)
{
	if(WIFI_Mode_FLAG == 0)
	{
		u2_printf("AT+CWMODE=3\r\n");//����Ϊ APģʽ
	
		Delay_ms(500);
		
		u2_printf("AT+RST\r\n");//����
		Delay_ms(1500);
		
		u2_printf("AT+CIPAP=\"192.168.x.x\"\r\n");//����IP:192.168.x.x
		Delay_ms(500);

		u2_printf("AT+CWSAP=\"FHQ\",\"12345678\",5,0\r\n");//����wifi������CZJ������12345678�����5����ͬʱ���ӣ�����ʱ�������룻
		Delay_ms(500);
		
		u2_printf("AT+CIPMUX=1\r\n");//����������
		Delay_ms(500);
		
		u2_printf("AT+CIPSERVER=1,8080\r\n");//���ö˿�8080
		Delay_ms(500);

//		u2_printf("AT+CIPSEND=0,5\r\n");
		
//		AT+CIPSEND=0,5
//		hello,tcp
	}
	if(WIFI_Mode_FLAG == 1)
	{

		u2_printf("AT+CWMODE=1\r\n");//����Ϊ softAP+station ����ģʽ
		Delay_ms(500);
;
		u2_printf("AT+RST\r\n");//����;
		Delay_ms(1500);

		u2_printf("AT+CWJAP=\"Mirror_zero\",\"11111111\"\r\n");//����wifi
		Delay_ms(3000);

		u2_printf("AT+CIPMUX=0\r\n");//����������
		Delay_ms(500);

		u2_printf("AT+CIPSTART=\"TCP\",\"xxx.xxx.xxx.xxx\",50021\r\n");//���ӷ�������Э��TCP��IP��ַxxx.xxx.xxx.xxx���˿�50021
		Delay_ms(3000);

		u2_printf("AT+CIPMODE=1\r\n");;
		Delay_ms(500);

		u2_printf("AT+CIPSEND\r\n");//����͸��
		Delay_ms(500);

	}

}






































/*
//����ȫ�ֱ���
volatile uint32_t esp8266_transparent_transmission_sta=0;

//����ͨ�Ŵ���
void esp8266_init(void)
{
	uart2_init(115200); 	//���ڳ�ʼ��
}

// ����WiFi�����ȵ� 
int32_t Esp8266_Init(void)
{
	int32_t ret;
	
	// esp8266_wifi���ô���1ͨ��,ǰ�������ô���1
	esp8266_init();

	Delay_ms(500);	
	
	// �˳�͸��ģʽ����������ATָ��
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("001: esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	Delay_ms(500);	
	printf("001: esp8266_exit_transparent_transmission success\r\n");
	Delay_ms(1000);
	
	
	// ��λģ��
	ret=esp8266_reset();
	if(ret)
	{
		printf("002: esp8266_reset fail\r\n");
		return -2;
	}
	Delay_ms(500);	
	printf("002: esp8266_reset success\r\n");
	Delay_ms(1000);

	
	// �رջ���
	ret=esp8266_enable_echo(0);
	if(ret)
	{
		printf("003: esp8266_enable_echo(0) fail\r\n");
		return -3;
	}	
	Delay_ms(500);	
	printf("003: esp8266_enable_echo(0)success\r\n");
	Delay_ms(1000);

	// �����ȵ�
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
//����ATָ��
void esp8266_send_at(char *str)
{
	//��ս��ջ�����
	memset((void *)USART2_RX_BUF,0, sizeof USART2_REC_LEN);
	
	//��ս��ռ���ֵ
	USART2_RX_STA = 0;	
	
	USART_sendstr(USART, str);	
}

// ���ҽ������ݰ��е��ַ��� 
int32_t esp8266_find_str_in_rx_packet(char *str,uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&USART2_RX_BUF;
	
	//�ȴ����ڽ�����ϻ�ʱ�˳���strstr()Ѱ����Ӧ�ַ��������δ�ҵ��򷵻� Null;
	while((strstr(src,dest)==NULL) && timeout) //while(�ҵ��� ��=  NULL && timeout == 0),�˳�ѭ����
	{		
		Delay_ms(1);
		timeout--;
	}

	if(timeout) 
		return 0;	//���ҵ����������
		                    
	return -1; 
}

// ���ESP8266�Ƿ����� 
int32_t esp8266_check(void)
{
	esp8266_send_at(" AT\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}

//  ��λ 
int32_t esp8266_reset(void)
{
	esp8266_send_at(" AT+RST\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000))
		return -1;

	return 0;
}


//  ���Դ򿪻�ر� 
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

//  �˳�͸��ģʽ 
int32_t esp8266_exit_transparent_transmission (void)
{
	esp8266_send_at ("+++");
	
	//�˳�͸��ģʽ��������һ��ATָ��Ҫ���1��
	Delay_ms(1000); 
	
	//��¼��ǰesp8266�����ڷ�͸��ģʽ
	esp8266_transparent_transmission_sta = 0;

	return 0;
}

//  ����͸��ģʽ 
int32_t esp8266_entry_transparent_transmission(void)
{
	//����͸��ģʽ
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	Delay_ms(1000);Delay_ms(1000);

	//��������״̬
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -2;
	Delay_ms(1000);Delay_ms(1000);

	//��¼��ǰesp8266������͸��ģʽ
	esp8266_transparent_transmission_sta = 1;
	return 0;
}

//  �������״̬ 
int32_t esp8266_check_connection_status(void)
{
	esp8266_send_at("AT+CIPSTATUS\r\n");
	
	if(esp8266_find_str_in_rx_packet("STATUS:3",10000))
		if(esp8266_find_str_in_rx_packet("OK",10000))
			return -1;

	return 0;
}


 * ���ܣ������ȵ�
 * ������
 *         ssid:�ȵ���
 *         pwd:�ȵ�����
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. WIFI�������벻��ȷ
 *         2. ·���������豸̫��,δ�ܸ�ESP8266����IP
 
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
#if 0	
	//������ʹ������sprintf��ռ�ù����ջ
	char buf[128]={0};
	
	//����ΪSTATIONģʽ	
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
	//����ΪAP+Stationģʽ	
	esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -1;

	esp8266_send_at("AT+CIPMUX=0\r\n");
	if(esp8266_find_str_in_rx_packet("OK",1000))
		return -2;

	//�����ȵ�
	esp8266_send_at("AT+CWJAP_CUR="); 
	esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
	esp8266_send_at(",");	
	esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
	esp8266_send_at("\r\n");
	//�����ȵ㣬��صȴ�����ָ���WIFI GOT IP����ʾ���ӳɹ����ٷ��������ָ�
	while(esp8266_find_str_in_rx_packet("WIFI GOT IP",5000));
#endif
	return 0;
}


 * ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
 * ������
 *         mode:Э������ "TCP","UDP"
 *         ip:Ŀ�������IP
 *         port:Ŀ���Ƿ������˿ں�
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. Զ�̷�����IP�Ͷ˿ں�����
 *         2. δ����AP
 *         3. �������˽�ֹ���(һ�㲻�ᷢ��)
 
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{

#if 0	
	//ʹ��MQTT���ݵ�ip��ַ������������ʹ�����·�����������ջ���
	//AT+CIPSTART="TCP","a10tC4OAAPc.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883�����ַ���ռ���ڴ������
	
	char buf[128]={0};
	
	//���ӷ�����
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

//  �Ͽ������� 
int32_t esp8266_disconnect_server(void)
{
	esp8266_send_at("AT+CIPCLOSE\r\n");
		
	if(esp8266_find_str_in_rx_packet("CLOSED",2000))
		if(esp8266_find_str_in_rx_packet("OK",2000))
			return -1;

	return 0;	
}

// ʹ�ܶ����� 
int32_t esp8266_enable_multiple_id(uint32_t b)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPMUX=%d\r\n", b);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

//  ���������� 
int32_t esp8266_create_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=1,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}

///�رշ����� 
int32_t esp8266_close_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=0,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))
		return -1;
	
	return 0;
}



����Ϊ�򵥿������Esp8266_WiFiģ��ΪAPģʽ���й�����������AP������

void Wifi_Init(void)
{
	USART_sendstr(USART,"AT+CWMODE=3\r\n");//����Ϊ softAP+station ����ģʽ
	Delay_ms(500);
	
	USART_sendstr(USART,"AT+RST\r\n");//����
	Delay_ms(1500);
	
	USART_sendstr(USART,"AT+CIPAP=\"192.168.1.1\"\r\n");//����IP:192.168.1.1
	Delay_ms(500);

	USART_sendstr(USART,"AT+CWSAP=\"FHQ\",\"12345678\",5,0\r\n");//����wifi������CZJ������12345678�����5����ͬʱ���ӣ�����ʱ�������룻
	Delay_ms(500);
	
	USART_sendstr(USART,"AT+CIPMUX=1\r\n");//����������
	Delay_ms(500);
	
	USART_sendstr(USART,"AT+CIPSERVER=1,8080\r\n");//���ö˿�8080
	Delay_ms(500);
}

//wifiģ�鷢�����---ÿ�ι̶�����num���ֽ�
void wifisend(char *buf,int num)
{
	//ÿ��wifiģ�鷢�����ݵ�ʱ�򣬶����ȷ���һ���̶�ǰ׺
	char sendfront[32];												//����ǰ׺�Ļ�����
	sprintf(sendfront,"AT+CIPSEND=0,%d\r\n",num);					//����ַ���
	
	USART_sendstr(USART,sendfront);
	Delay_ms(5);
	USART_sendstr(USART,buf);
}

//����len���ȵ��ַ���
void USART_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len)
{ 
	while(Len--)
	{				                          			//�ж��Ƿ񵽴��ַ���������
	    USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); 	//�ȴ�������
	}
}

//����һ���������ַ���
void USART_sendstr(USART_TypeDef* USARTx, char *Data) // "shijienameda"
{ 
	//ѭ������1���ֽڣ�ֱ��׼�����͵��ֽ���'\0',Ҳ�����ַ���ĩβ��ֹͣ����
	while(*Data!=0)
	{				                        
		USART_SendData(USARTx, *Data);
		Data++;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}

*/
