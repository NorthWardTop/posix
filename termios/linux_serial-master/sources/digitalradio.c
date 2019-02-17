/*
 * digitalradio.c
 *
 *  Created on: Oct 19, 2016
 *      Author: wangbo
 */

#include <stdio.h>
#include <string.h>

/*转换int或者short的字节顺序，该程序arm平台为大端模式，地面站x86架构为小端模弿*/
#include <byteswap.h>

/*open() close()所需头文仿*/
#include <unistd.h>

#include "digitalradio.h"
#include "uart.h"

#define RADIO_RECV_BUF_LEN 512

#define RADIO_RECV_HEAD1 0
#define RADIO_RECV_HEAD2 1
#define RADIO_RECV_LEN 2
#define RADIO_RECV_CNT 3
#define RADIO_RECV_SYSID 4
#define RADIO_RECV_TYPE 5
#define RADIO_RECV_DATA 6
#define RADIO_RECV_CHECKSUM 7

static int radio_recv_state = 0;

/**
 * @description: 串口初始化函数
 * @param {type} 
 * @return: 
 */
int radio_uart_init()
{
    uart_device.uart_name = UART_RADIO;

    uart_device.baudrate = UART_RADIO_BAUD;
    uart_device.databits = UART_RADIO_DATABITS;
    uart_device.parity = UART_RADIO_PARITY;
    uart_device.stopbits = UART_RADIO_STOPBITS;
    //打开串口名将串口号返回(下标)
    uart_device.uart_num = open_uart_dev(uart_device.uart_name);
    //绑定读函数
    uart_device.ptr_fun = read_radio_data;
    //设置串口的一些选项(名称,波特率,数据宽度,校验,停止位)
    set_uart_opt(uart_device.uart_name,
                 uart_device.baudrate,
                 uart_device.databits,
                 uart_device.parity,
                 uart_device.stopbits);

    create_uart_pthread(&uart_device);

    return 0;
}

/*
 * 函数需要修政 * 用到了global_bool_boatpilot变量
 *
 */
int read_radio_data(unsigned char *buf, unsigned int len)
{
    static unsigned char _buffer[RADIO_RECV_BUF_LEN];

    static unsigned char _pack_recv_type;
    static int _pack_recv_cnt = 0;
    static int _pack_recv_len = 0;
    static unsigned char _pack_recv_buf[RADIO_RECV_BUF_LEN];
    static int _pack_buf_len = 0;
    static unsigned char _checksum = 0;

    int _length;
    int i = 0;
    unsigned char c;
    unsigned char _sysid;

    // _buffer 通过串口读取的数据
    memcpy(_buffer, buf, len);

    /*显示通过电台收到的数捿*/
#if 1
    printf("radio data buf=\n");
    for (i = 0; i < len; i++)
    {
        printf("%c", buf[i]);
    }
    printf("\n");
#endif

    _length = len;
    for (i = 0; i < _length; i++)
    {
        c = _buffer[i];

        //解析应用层数据协议(协议状态机)
        //aa 55 接受长度 接受计数 接受id 接受类型 接受数据 接受校验
        //第1,2状态用来辨别包
        switch (radio_recv_state)
        {
        case RADIO_RECV_HEAD1:
            if (c == 0xaa) //第一状态时候如果字符为aa,则是标准头,转化为第二状态
            {
                radio_recv_state = RADIO_RECV_HEAD2;
                _checksum = c; //第一状态开始计算校验值
            }
            break;
        case RADIO_RECV_HEAD2:
            if (c == 0x55) //第二字符为55
            {              //修改到第三状态
                radio_recv_state = RADIO_RECV_LEN;
                _checksum += c;
            }
            else //接受错误, 返回到第一状态
            {
                radio_recv_state = RADIO_RECV_HEAD1;
            }
            break;
        case RADIO_RECV_LEN:    //第三长度状态
            _pack_recv_len = c; //设置长度值
            radio_recv_state = RADIO_RECV_CNT;
            _checksum += c;
            _pack_buf_len = 0;
            break;
        case RADIO_RECV_CNT:    //第四状态计数
            _pack_recv_cnt = c; //设置计数值
            radio_recv_state = RADIO_RECV_SYSID;
            _checksum += c;
            _pack_buf_len = 0;
            break;
        case RADIO_RECV_SYSID: //第四状态id
            _sysid = c;        //设置id
            if (0 != _sysid)   //id为0继续在第四状态
            {
                radio_recv_state = RADIO_RECV_TYPE;
            }
            _checksum += c;
            _pack_buf_len = 0;
            break;
        case RADIO_RECV_TYPE:    //第五状态类型
            _pack_recv_type = c; //保存类型(状态输出)
            radio_recv_state = RADIO_RECV_DATA;
            _checksum += c;
            _pack_buf_len = 0;
            break;
        case RADIO_RECV_DATA:                  //第六接受数据
            _pack_recv_buf[_pack_buf_len] = c; //保存至第len个字符
            _checksum += c;
            _pack_buf_len++;                     //每到这个状态,保存字符后,len++
            if (_pack_buf_len >= _pack_recv_len) //当缓冲保存长度>=包中的接受长度
            {
                radio_recv_state = RADIO_RECV_CHECKSUM; //下一状态为校验状态
            }
            break;
        case RADIO_RECV_CHECKSUM: //第七校验状态(并解析接受类型逻辑)
            if (_checksum == c)   //所计算校验值 是否等于 发来的校验值
            {
                switch (_pack_recv_type) //解析类型
                {
                    /*
                    * 经过帧头校验后的处理程序写在这里
                    */
#if 0
                    case COMMAND_GCS2AP_CMD:
                        if (_pack_recv_len == sizeof(gcs2ap_cmd))
                        {
                            memcpy(&gcs2ap_cmd, _pack_recv_buf, _pack_recv_len);
                            global_bool_boatpilot.bool_get_gcs2ap_cmd = TRUE;
                        }
                        radio_recv_state = 0; //完成后回归状态
                        global_bool_boatpilot.wgcs2ap_cmd_cnt++;
                        break;
#endif
                default:
                    break;
                }
            }
            else
            {
                //send_ap2gcs_ack(_pack_recv_type, _pack_recv_cnt, 0x01);
                radio_recv_state = 0; //完成后回归状态
            }
        }
    }

    return 0;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int send_radio_data(unsigned char *buf, unsigned int len)
{
    uart_device.uart_name = UART_RADIO;
    send_uart_data(uart_device.uart_name, (char *)buf, len);

    return 0;
}

int radio_uart_close()
{
    uart_device.uart_name = UART_RADIO;
    close_uart_dev(uart_device.uart_name);

    return 0;
}
