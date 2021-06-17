/*
 * @Description: 
 * @Version: 
 * @Autor: wigen
 * @Date: 2021-05-21 16:37:24
 * @LastEditors: wigen
 * @LastEditTime: 2021-06-08 11:19:46
 */
#ifndef __MY_MODBUS_H
#define __MY_MODBUS_H

#include "main.h"

#define RS485_RX  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET)
#define RS485_TX  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET)
#define BUF_MAXSIZE  100
#define REG_MAXSIZE  32

#define undefined_func_code   0x01  //δ���幦���룬ѯ���н��յ��Ĺ������ǲ�������Ĳ���
#define illegal_reg_address   0x02  //�Ƿ��ļĴ������ݵ�ַ�����յ������ݵ�ַ�ǲ�������ĵ�ַ
#define illegal_reg_data   0x03     //�Ƿ�����ֵ���Ĵ����б��ύ�洢����������һ��Ӧ�ó�������֮���ֵ
#define slave_device_busy  0x04     //���豸æ�����ڴ�����һ������

typedef enum {
    mb_init_flag = 0,
    mb_idle_flag,
    mb_recv_busy_flag,
    mb_data_prasing_flag
}   mb_work_flag_t;




typedef struct {
    uint8_t address;                //������Ϊ�ӻ��ĵ�ַ
    mb_work_flag_t work_flag;       //����״̬
    uint8_t recvbuf[BUF_MAXSIZE];    //���ջ�����
    uint8_t sendbuf[BUF_MAXSIZE];    //���ͻ�����
    uint32_t regbuf[REG_MAXSIZE];     //�Ĵ���������

    uint8_t  frame_timeout_flag;    //֡�����ʱ״̬ ���� 3.5���ַ� = frame_interval(us)
    uint32_t frame_interval;        //��ʱ������ж�ʱ��
	uint8_t  tim_cnt;               //tim_cnt*��ʱ������ж�ʱ�� = 3.5���ַ�ʱ��

} modbus_t;

int modbus_init(uint8_t slave_addr, UART_HandleTypeDef * huart, TIM_HandleTypeDef * tim, modbus_t * p_mb);
void mb_recv_handler(UART_HandleTypeDef * huart, TIM_HandleTypeDef * tim, modbus_t * p_mb);
void mb_tim_handler(modbus_t * p_mb, variable_t * p_var, UART_HandleTypeDef * huart, TIM_HandleTypeDef * htim);
void mb_data_prase(modbus_t * p_mb, variable_t * p_var, UART_HandleTypeDef * huart);
void read_holdregister(modbus_t * p_mb, variable_t * p_var, UART_HandleTypeDef * huart);
void write_single_register(modbus_t * p_mb, variable_t * p_var, UART_HandleTypeDef * huart);
void write_multi_register(modbus_t * p_mb, variable_t * p_var, UART_HandleTypeDef * huart);
void error_handle(modbus_t * p_mb, uint8_t err_code, UART_HandleTypeDef * huart);
void mb_send_frame(modbus_t * p_mb, UART_HandleTypeDef * huart, uint8_t len);
unsigned short CRC16 (unsigned char * puchMsg, unsigned short usDataLen );
#endif
