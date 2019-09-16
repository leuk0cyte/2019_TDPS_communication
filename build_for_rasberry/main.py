##########################################################
# Coding = utf-8
# Written by leuk0cyte, 2019
# Title  : TDPS Main program in raspbarry
# TEAM   : 23
##########################################################
import os
import time
from motor_control import *
from mbed_serial import *
from constant_definition import *
from get_time import *
####################初始化mbed通信#########################
mbed = MBED()
##########################################################

# 触发中断，并汇报错误码
def interrupt():
    raise Exception(f'An error occured, error code {error_code}')

def init(): # 主系统初始化
    mbed.tx(MBED_INIT) #初始化mbed系统
    mbed_return()

         
def mbed_return(): # 通用处理mbed返回值处理程序
    # 两秒为周期，轮询返回值，超时报错
    time_start = time.time()
    while True:
        time.sleep(2)
        if (time.time()-time_start>MBED_MAX_RETURN_TIME): #mbed响应超时
            error_code = MBED_RESPONSE_TIMEOUT
            interrupt()
        elif mbed.inBuffer()>0: # 退出循环条件：检测到缓冲器中有数据
            break

    mbed_response = mbed.rx() #接收数据
    len_of_response = len(mbed_response)
    if len_of_response == 1:    # 没有读取到mbed的返回值？ （这种错误真的会出现么……）
        error_code = MBED_READ_RESPONSE_ERROR
        interrupt()
    elif mbed_response[0] = MBED_FIELD:  # mbed返回：命令执行错误
        error_code = mbed_response[1:len_of_response-1]
        interrupt()
    else:
        return mbed_response



    
def stage0():
    motor_drive(motor_slow)
    time.sleep(4)
    motor_angle(-90)

def mbed_white_balance():
    mbed.tx(MBED_WHITE_BALANCE)
    time.sleep(5)
    mbed_return()

def mbed_rgb():
    mbed.tx(MBED_RGB)
    time.sleep(3)
    colour_raw = mbed_return()
    colour_len = len(colour_raw)
    colour = colour_raw[colour_len-1:colour_len] # 取最后一位
    if colour != 'r' and colour != 'g' and colour != 'b':
        error_code = MBED_READ_COLOUR_ERROR
        interrupt()
    return colour

def mbed_arm():
    mbed.tx(MBED_ARM)
    time.sleep(20)
    mbed_return()

def mbed_radio():
    mbed.tx(MBED_RADIO)  # 发送433通信指令
    time.sleep(1)
    CST = get_time()  
    mbed.tx(CST)    # 获取当前时间并发送
    time.sleep(20)
    mbed_return()   # 等待mbed接收到电脑的确认信息
    
    
if __name__ == "__main__":
    init()

    def run_stage(stage_no):
        if stage_no == 0:

        pass
    