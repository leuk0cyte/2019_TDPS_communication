##########################################################
# Coding = utf-8
# Structure designed by You Hao, 2019
# Coder  : Hao You, Chao Deng, Haoyuan Su
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
############Initialize mbed communication#################
mbed = MBED()
##########################################################

#NAME       : get_time
#INTROCTION : A function to get 24-clock time like hh:mm:ss
#INPUT      : None
#OUTPUT     : 8-byte string
#PAKAGE RELY: time
def get_time():
    return(time.strftime('%H:%M:%S',time.localtime(time.time())))

# Trigger interrupt and report error code
def interrupt():
    raise Exception('An error occured, error code {}'.form(error_code))


def init():  # Main system initialization
    mbed.tx(MBED_INIT)  # Initialize the mbed system
    mbed_return()

         
def mbed_return():  # Generic processing mbed return value handler
    # Two seconds is the cycle, polling the return value, timeout error
    time_start = time.time()
    while True:
        time.sleep(2)
        if (time.time()-time_start) > MBED_MAX_RETURN_TIME:  # Mbed response timeout
            error_code = MBED_RESPONSE_TIMEOUT
            interrupt()
        elif mbed.inBuffer() > 0:  # Exit loop condition: detected data in buffer
            break
    mbed_response = mbed.rx()  # Receive data
    len_of_response = len(mbed_response)
    if len_of_response == 1:    # Did not read the return value of mbed
        error_code = MBED_READ_RESPONSE_ERROR
        interrupt()
    elif mbed_response[0] == MBED_FIELD:  # Mbed return: command execution error
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
    mbed.tx(MBED_RADIO)  # Send remote communication command
    time.sleep(1)
    CST = get_time()  
    mbed.tx(CST)    # Get current time and send
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
        print('turn right 90')
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
    print('turn right 90')
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
                print('confirm: %d' % w)
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
    while T == 0:  # Start running after the first left turn
        motor_drive(motor_slow)
        time.sleep(0.1)
        wall_following(0.8, 0.35, 0.25, 0.03)
        
    motor_angle(-5)
    motor_drive(motor_slow)
    time.sleep(3)
    print('T=1')
    while T == 1:  # After the second right turn, start running
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
            print('confirm: %d' % e)
            if e == 2:
                motor_start()
                motor_drive(0)
                time.sleep(2)
                motor_angle(-90)
                time.sleep(0.35)
                print('turn right 90')
                motor_drive(motor_slow)
                T = T + 1

    print('T=2')
    while T == 2:  # Start running after the first left turn
        motor_drive(motor_slow)
        time.sleep(0.1)
        wall_following(0.8, 0.35, 0.25, 0.03)

    print('T=3')
    while T == 3:  # After the second right turn, start running
        motor_drive(motor_slow)
        time.sleep(0.1)
        distance0 = checkdist(0)
        if distance0 > 0.45:
            wall_following(100, 0.3, 0.25, 0.03)
        else:
            j = j + 1
            print('conferm: %d' % j)
            motor_break()
            time.sleep(0.5)
            if j == 2:
                motor_start()
                motor_drive(0)
                time.sleep(2)
                motor_angle(-90)
                time.sleep(0.3)
                corner_adjust(0.01)
                print('turn left 90')
                motor_start()
                motor_drive(motor_slow)
                T = T + 1

    print('T=4')
    while T == 4:  # After the second left turn, start running, go straight after a period of feeding
        motor_drive(motor_slow)
        distance0 = checkdist(0)
        print('distance0 = %0.2f m' % distance0)
        if distance0 < 0.45:
            k = k + 1
            motor_brake()
            time.sleep(0.5)
            print('confirm: %d' % k)
            if k == 2:
                motor_brake()
                mbed_arm()  # feeding
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
