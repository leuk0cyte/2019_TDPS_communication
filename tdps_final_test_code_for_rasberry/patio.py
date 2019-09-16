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
from ultrasonic_navigation import *
from computer_vision import *
from corner_adjust import *
####################初始化mbed通信#########################
mbed = MBED()
##########################################################


# 触发中断，并汇报错误码
def interrupt():
    raise Exception('An error occured, error code {}'.form(error_code))


def init():  # 主系统初始化
    mbed.tx(MBED_INIT)  # 初始化mbed系统
    mbed_return()

         
def mbed_return():  # 通用处理mbed返回值处理程序
    # 两秒为周期，轮询返回值，超时报错
    time_start = time.time()
    while True:
        time.sleep(2)
        if (time.time()-time_start) > MBED_MAX_RETURN_TIME:  # mbed响应超时
            error_code = MBED_RESPONSE_TIMEOUT
            interrupt()
        elif mbed.inBuffer() > 0:  # 退出循环条件：检测到缓冲器中有数据
            break
    mbed_response = mbed.rx()  # 接收数据
    len_of_response = len(mbed_response)
    if len_of_response == 1:    # 没有读取到mbed的返回值？ （这种错误真的会出现么……）
        error_code = MBED_READ_RESPONSE_ERROR
        interrupt()
    elif mbed_response[0] == MBED_FIELD:  # mbed返回：命令执行错误
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
    colour = colour_raw[colour_len-1:colour_len]
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
    mbed_return()  
    
    
def wall_following(l_region, m_region, r_region, tolerance):
    global distance0, distance1, distance2, T
    distance1 = checkdist(1)
    distance2 = checkdist(2)
    print('distance1 = %0.2f m' % distance1)
    print('distance2 = %0.2f m' % distance2)
    if distance1 < r_region:
        if distance1 - distance2 > tolerance:
            motor_angle(5)
        else:
            motor_angle(-2)
    elif (distance1 > m_region) & (distance1 < l_region):
        if distance2 - distance1 > tolerance:
            motor_angle(-5)
        else:
            motor_angle(2)
    elif (distance1 >= r_region) & (distance1 <= m_region):
        motor_drive(motor_slow)
    else:
        motor_drive(motor_slow)
        time.sleep(0.5)
        motor_angle(90)
        print('右转90')
        motor_neutral()
        time.sleep(3)
        motor_start()
        T = T + 1
        
    


i = 0
j = 0
k = 0
q = 0
w = 0
e = 0
r = 0

init()
mbed_white_balance()

stage = 0
motor_start()
motor_drive(motor_slow)
time.sleep(10.4)
motor_neutral()
time.sleep(5)
colour = mbed_rgb()
motor_start()
motor_drive(motor_slow)
stage = stage + 1
while stage == 1:
    if colour == 'r':
        motor_drive(motor_slow)
        time.sleep(1.4)
        motor_angle(-45)
        motor_drive(motor_slow)
        time.sleep(10)
        motor_angle(91)
        motor_drive(motor_slow)
        time.sleep(12.7)
        motor_angle(45)
        motor_neutral()
        stage = stage + 1
    elif colour == 'g':
        motor_drive(motor_slow)
        time.sleep(1.4)
        motor_angle(91)
        motor_drive(motor_slow)
        time.sleep(17)
        motor_neutral()
        stage = stage + 1
    elif colour == 'b':
        motor_back()
        time.sleep(2)
        motor_neutral()
        motor_start()
        motor_angle(-90)
        motor_angle(-60)
        time.sleep(0.4)
        motor_drive(motor_slow)
        time.sleep(11)
        motor_angle(45)
        time.sleep(0.6)
        motor_drive(motor_slow)
        time.sleep(12.5)
        motor_angle(-45)
        time.sleep(0.2)
        stage = stage + 1
    else:
        colour = mbed_rgb()


while stage == 2:
    # csb
    T = -1
    print('T=-1')
    motor_start()
    motor_drive(motor_slow)
    time.sleep(6)
    motor_angle(90)
    print('右转90')
    motor_neutral()
    time.sleep(2)
    motor_start()
    
    while T == -1:
        distance0 = checkdist(0)
        print('distance0 = %0.2f m' % distance0)
        if distance0 < 0.55:
                w = w + 1
                motor_brake()
                time.sleep(0.5)
                print('w出错次数: %d' % w)
                if w == 2:
                    motor_angle(-90)
                    motor_angle(-5)
                    motor_angle(-5)
                    motor_angle(-5)
                    print('zuo90')
                    motor_neutral()
                    time.sleep(2)
                    motor_start()
                    T = T + 1
        else:
                motor_drive(motor_slow)
    
    print('T=0')
    while T == 0:  # 第一次左转完开始运行
        motor_drive(motor_slow)
        time.sleep(0.1)
        wall_following(0.8, 0.35, 0.25, 0.03)
        
    motor_angle(-5)
    motor_drive(motor_slow)
    time.sleep(3)
    print('T=1')
    while T == 1:  # 第二次右转完开始运行
        motor_drive(motor_slow)
        time.sleep(1)
        distance0 = checkdist(0)
        print('distance0 = %0.2f m' % distance0)
        if distance0 > 0.7:
            wall_following(100, 0.8, 0.7, 0.03)
        else:
            motor_brake()
            time.sleep(0.5)
            e = e + 1
            print('e出错次数: %d' % e)
            if e == 2:
                motor_start()
                motor_drive(0)
                time.sleep(2)
                motor_angle(-90)
                time.sleep(0.35)
                print('左转90')
                motor_drive(motor_slow)
                T = T + 1

    print('T=2')
    while T == 2:  # 第一次左转完开始运行
        motor_drive(motor_slow)
        time.sleep(0.1)
        wall_following(0.8, 0.35, 0.25, 0.03)

    print('T=3')
    while T == 3:  # 第二次右转完开始运行
        motor_drive(motor_slow)
        time.sleep(0.1)
        distance0 = checkdist(0)
        if distance0 > 0.45:
            wall_following(100, 0.3, 0.25, 0.03)
        else:
            j = j + 1
            print('j出错次数: %d' % j)
            motor_break()
            time.sleep(0.5)
            if j == 2:
                motor_start()
                motor_drive(0)
                time.sleep(2)
                motor_angle(-90)
                time.sleep(0.3)
                corner_adjust(0.01)
                print('左转90')
                motor_start()
                motor_drive(motor_slow)
                T = T + 1

    print('T=4')
    while T == 4:  # 第二次左转完开始运行，直走一段后投石
        motor_drive(motor_slow)
        distance0 = checkdist(0)
        print('distance0 = %0.2f m' % distance0)
        if distance0 < 0.45:  # 离信标xxx m处投石
            k = k + 1
            motor_brake()
            time.sleep(0.5)
            print('k出错次数: %d' % k)
            if k == 2:
                motor_brake()
                mbed_arm()  # 投石ing
                T = T + 1
        else:
            motor_drive(motor_slow)
            time.sleep(0.3)
            wall_following(100, 0.1, 0.08, 0.01)

    print('T=5')
    while T == 5:
        motor_drive(motor_slow)
        time.sleep(0.2)
        motor_angle(-60)
        time.sleep(0.6)
        motor_drive(motor_slow)
        time.sleep(50)
        motor_angle(-5)
        time.sleep(0.4)
        motor_drive(motor_slow)
        time.sleep(25)
        T = T + 1
        
        stage = stage + 1

while stage == 3:
    motor_neutral()
    mbed_radio()
    motor_start()
    motor_drive(motor_mid)
    time.sleep(20)
    motor_neutral()
    stage = stage + 1

motor_neutral()
