/****************
//mtf01
//����޸�ʱ��:08/12/2023

****************/

//#pragma once

#ifndef _MTF01_H_ 
#define _MTF01_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MICOLINK_MSG_HEAD            0xEF
#define MICOLINK_MAX_PAYLOAD_LEN     64
#define MICOLINK_MAX_LEN             MICOLINK_MAX_PAYLOAD_LEN + 7

/*
    ��ϢID����
*/
enum
{
    MICOLINK_MSG_ID_RANGE_SENSOR = 0x51,     // ��ഫ����
};

/*
    ��Ϣ�ṹ�嶨��
*/
typedef struct
{
    uint8_t head;                      
    uint8_t dev_id;                          
    uint8_t sys_id;						
    uint8_t msg_id;                        
    uint8_t seq;                          
    uint8_t len;                               
    uint8_t payload[MICOLINK_MAX_PAYLOAD_LEN]; 
    uint8_t checksum;                          

    uint8_t status;                           
    uint8_t payload_cnt;                       
} MICOLINK_MSG_t;

/*
    ���ݸ��ض���
*/
//#pragma pack (1)
// ��ഫ����
typedef struct
{
    uint32_t    time_ms;						// ϵͳʱ�� ms
    uint32_t    distance;						// ����(mm) ��СֵΪ10��0��ʾ���ݲ�����
    uint8_t     strength;						// �ź�ǿ��
    uint8_t     precision;					// ����
    uint8_t     tof_status;					// ״̬
    uint8_t     reserved1;					// Ԥ��
    int16_t     flow_vel_x;					// �����ٶ�x��
    int16_t     flow_vel_y;					// �����ٶ�y��
    uint8_t     flow_quality;				// ��������
    uint8_t     flow_status;				// ����״̬
    uint16_t    reserved2;					// Ԥ��
} MICOLINK_PAYLOAD_RANGE_SENSOR_t;

bool micolink_parse_char(MICOLINK_MSG_t* msg, uint8_t data);
void micolink_decode(uint8_t data);
bool micolink_check_sum(MICOLINK_MSG_t* msg);

//#pragma pack ()
#endif // _MTF01_H_ 



