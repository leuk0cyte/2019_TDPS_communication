import os
import serial

class MBED:
    #初始化与mbed的串口通讯
    def __init__(self):
        #建立一个包含mbed地址的文件对象
        with os.popen('python -m serial.tools.list_ports') as read_serial_port:
            #提取文件的前12个字符，此为mbed在linux系统下的地址
            serial_port = read_serial_port.read()[0:12]
        
        #建立串口通讯对象mbed
        mbed = serial.Serial(serial_port,9600,timeout=0.5)
        self.mbed = mbed
        #打开串口
        if mbed.isOpen() != 1:
            mbed.open()
    
    #从缓冲区接收数据，接收错误返回1，成功返回接收字段
    #注意：调用本函数接收数据后，串口通讯的缓冲区即被清空。
    def rx(self):
        if self.mbed.inWaiting()==0:
            return 1
        else:
            read_buffer = self.mbed.read(self.mbed.inWaiting())
            return read_buffer
        
    #传输数据给mbed
    def tx(self,send_buffer):
        #对将要传输的字符串使用gbk编码
        send_buffer = send_buffer.encode(encoding="gbk",errors="ignore")
        #发送
        self.mbed.write(b'%s\n'%send_buffer)