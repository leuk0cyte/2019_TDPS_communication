import RPi.GPIO as GPIO
import time

# 先附上树莓派3B+IO口网址：http://shumeipai.nxez.com/raspberry-pi-pins-version-40
# 树莓派PWM设定的网址：https://www.jianshu.com/p/4e9e0ecfa6aa

#############################################################################
##########################引脚定义############################################
GPIO.setmode(GPIO.BCM)               # 使用树莓派BCM引脚排列，而不是board

INT1 = 6                            # 对应树莓派3b+的引脚为：31,29,28,26,27,24
INT2 = 5
INT3 = 1
INT4 = 7
PWMA = 0
PWMB = 8

GPIO.setwarnings(False)              # 消除程序报错警告
GPIO.setup(INT1, GPIO.OUT)           # 配置IO口功能
GPIO.setup(INT2, GPIO.OUT)
GPIO.setup(INT3, GPIO.OUT)
GPIO.setup(INT4, GPIO.OUT)
GPIO.setup(PWMA, GPIO.OUT)
GPIO.setup(PWMB, GPIO.OUT)

pwma = GPIO.PWM(PWMA, 10000)            # 配置PWM，电机驱动频率大约10k最佳
pwmb = GPIO.PWM(PWMB, 10000)
pwma.start(0)
pwmb.start(0)
##############################################################################
###################################参数定义####################################
motor_slow = 10                                 # 小车运行速度
motor_mid = 30
motor_fast = 50

RIGHT_MOTOR_PARAM = 1.2                # 分别定义左右电机的运转系数，使得两电机保持相同的速度
LEFT_MOTOR_PARAM  = 0.67               # 小于1
##############################################################################


def motor_right(degree1):                  # 右转，参数为转的角度
    dc1 = degree1/3                         # 调整电机转速差值，
    dc2 = degree1/10                        # 用于调节转弯半径
    pwma.ChangeDutyCycle(dc1)
    pwmb.ChangeDutyCycle(dc2)


def motor_left(degree2):                   # 左转，参数为转的角度
    dc1 = degree2/10                         # 调整电机转速差值，
    dc2 = degree2/3                         # 用于调节转弯半径
    pwma.ChangeDutyCycle(dc1)
    pwmb.ChangeDutyCycle(dc2)


def motor_drive(degree):                   # 用于驱动小车直行
    dc1 = degree*RIGHT_MOTOR_PARAM                     # 调整电机转速差值，
    dc2 = degree*LEFT_MOTOR_PARAM                      # 用于调节转弯半径
    pwma.ChangeDutyCycle(dc1)
    pwmb.ChangeDutyCycle(dc2)


def motor_start():                          # 配置IO口，使两个电机正转，小车直行
    GPIO.output(INT1, GPIO.LOW)
    GPIO.output(INT2, GPIO.HIGH)
    GPIO.output(INT3, GPIO.LOW)
    GPIO.output(INT4, GPIO.HIGH)
    motor_drive(20)   # 启动速度：20
    
    
def motor_back():                          # 配置IO口，使两个电机正转，小车直行
    GPIO.output(INT1, GPIO.HIGH)
    GPIO.output(INT2, GPIO.LOW)
    GPIO.output(INT3, GPIO.HIGH)
    GPIO.output(INT4, GPIO.LOW)
    motor_drive(15)   # 启动速度：10


def motor_brake():                      # 急刹车
    motor_drive(0)


def motor_neutral():                    # 自由停止，适用于转速很快时缓慢停下
    GPIO.output(INT1, GPIO.LOW)
    GPIO.output(INT2, GPIO.LOW)
    GPIO.output(INT3, GPIO.LOW)
    GPIO.output(INT4, GPIO.LOW)
    motor_drive(0)


def motor_angle(degree):              # 原地转动特定的角度，正值为右负值为左
    if degree == -45:                    # 左45度
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(1.2)
    elif degree == 45:                  # 右45度
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(1.1)
    elif degree == -60:                  # 左60度
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(1)
    elif degree == -90:                  # 左90度
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(1.5)
    elif degree == 90:                  # 右90度
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(2)
    elif degree == -2:                 # 左2度
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(0.1)
    elif degree == 2:                  # 右2度
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(0.1)
    elif degree == -5:  # 左5度
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(0.2)
    elif degree == 5:  # 右5度
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(0.15)
    elif degree == 91:  # 图像识别
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(2.4)
    else:               
        motor_drive(0)
        time.sleep(0.15)
    return


def motor_corner(degree):
    if degree == -2:  # 左2度
        GPIO.output(INT1, GPIO.HIGH)
        GPIO.output(INT2, GPIO.LOW)
        GPIO.output(INT3, GPIO.LOW)
        GPIO.output(INT4, GPIO.HIGH)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.1)
    elif degree == 2:  # 右2度
        GPIO.output(INT1, GPIO.LOW)
        GPIO.output(INT2, GPIO.HIGH)
        GPIO.output(INT3, GPIO.HIGH)
        GPIO.output(INT4, GPIO.LOW)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.1)
    elif degree == -5:  # 左5度
        GPIO.output(INT1, GPIO.HIGH)
        GPIO.output(INT2, GPIO.LOW)
        GPIO.output(INT3, GPIO.LOW)
        GPIO.output(INT4, GPIO.HIGH)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.2)
    elif degree == 5:  # 右5度
        GPIO.output(INT1, GPIO.LOW)
        GPIO.output(INT2, GPIO.HIGH)
        GPIO.output(INT3, GPIO.HIGH)
        GPIO.output(INT4, GPIO.LOW)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.2)
    else:
        motor_drive(0)
        time.sleep(0.5)

