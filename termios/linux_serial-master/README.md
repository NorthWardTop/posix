# linux_serial
Linux平台下创建串口线程并处理串口接收的程序模板
struct T_UART_DEVICE uart_device; 串口对象 
struct T_UART_DEVICE
{
    char *uart_name;
    int (*ptr_fun)(unsigned char *buf,unsigned int len);

    unsigned char uart_num;

    unsigned char databits;
    unsigned char parity;
    unsigned char stopbits;
    unsigned int  baudrate;
};

struct serial_data
{
	unsigned char head1;
	unsigned char head2;
}
serial_data * data = buf
