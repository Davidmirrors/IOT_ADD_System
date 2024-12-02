
#include "mtf01.h"
#include "usart.h"

/*


����޸�ʱ��:08/12/2023

������Чֵ��СΪ10(mm),Ϊ0˵����ʱ����ֵ������
�����ٶ�ֵ��λ��cm/s@1m
�ɿ���ֻ��Ҫ�������ٶ�ֵ*�߶ȣ����ɵõ���ʵˮƽλ���ٶ�
���㹫ʽ��ʵ���ٶ�(cm/s)=�����ٶ�*�߶�(m)
*/


void micolink_decode(uint8_t data)
{
    static MICOLINK_MSG_t msg;

    if(micolink_parse_char(&msg, data) == false)
        return;
    
    switch(msg.msg_id)
    {
        case MICOLINK_MSG_ID_RANGE_SENSOR:
        {
            MICOLINK_PAYLOAD_RANGE_SENSOR_t payload;
            memcpy(&payload, msg.payload, msg.len);

            /*
                �˴��ɻ�ȡ����������:
            
                ����        = payload.distance;
                ǿ��        = payload.strength;
                ����        = payload.precision;
                ����״̬    = payload.tof_status;
                �����ٶ�x�� = payload.flow_vel_x;
                �����ٶ�y�� = payload.flow_vel_y;
                ��������    = payload.flow_quality;	
                ����״̬    = payload.flow_status;
            */
					
            break;
        } 

        default:
            break;
        }
}

bool micolink_check_sum(MICOLINK_MSG_t* msg)
{
    uint8_t length = msg->len + 6;
    uint8_t temp[MICOLINK_MAX_LEN];
    uint8_t checksum = 0;
		static uint8_t i = 0;
    memcpy(temp, msg, length);

    for( i = 0; i<length; i++)
    {
        checksum += temp[i];
    }

    if(checksum == msg->checksum)
        return true;
    else
        return false;
}

bool micolink_parse_char(MICOLINK_MSG_t* msg, uint8_t data)
{
    switch(msg->status)
    {
    case 0:     //֡ͷ
        if(data == MICOLINK_MSG_HEAD)
        {
            msg->head = data;
            msg->status++;
						printf("inspect11");
        }
        break;
        
    case 1:     // �豸ID
        msg->dev_id = data;
        msg->status++;
				printf("inspect22");
        break;
    
    case 2:     // ϵͳID
        msg->sys_id = data;
        msg->status++;
				printf("inspect33");
        break;
    
    case 3:     // ��ϢID
        msg->msg_id = data;
        msg->status++;
				printf("inspect44");
        break;
    
    case 4:     // ������
        msg->seq = data;
        msg->status++;
				printf("inspect55");
        break;
    
    case 5:     // ���س���
        msg->len = data;
        if(msg->len == 0)
            msg->status += 2;
        else if(msg->len > MICOLINK_MAX_PAYLOAD_LEN)
            msg->status = 0;
        else
            msg->status++;
				
				printf("inspect66");
        break;
        
    case 6:     // ���ݸ��ؽ���
        msg->payload[msg->payload_cnt++] = data;
        if(msg->payload_cnt == msg->len)
        {
            msg->payload_cnt = 0;
            msg->status++;
						printf("inspect77");
        }
        break;
        
    case 7:     // ֡У��
        msg->checksum = data;
        msg->status = 0;
        if(micolink_check_sum(msg))
        {
						printf("inspect88");
            return true;
        }
        
    default:
        msg->status = 0;
        msg->payload_cnt = 0;
				printf("inspect99");
        break;
    }

    return false;
}

