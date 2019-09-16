import RPi.GPIO as GPIO
import time


#############################################################################
##########################Pin definition#####################################
GPIO.setmode(GPIO.BCM)               # Use Raspberry Pi BCM pinout instead of board

INT1 = 6                            # Corresponding to the Raspberry Pi 3b+ pins: 31, 29, 28, 26, 27, 24
INT2 = 5
INT3 = 1
INT4 = 7
PWMA = 0
PWMB = 8

GPIO.setwarnings(False)              # Eliminate program error warning
GPIO.setup(INT1, GPIO.OUT)           # Configure the IO port function.
GPIO.setup(INT2, GPIO.OUT)
GPIO.setup(INT3, GPIO.OUT)
GPIO.setup(INT4, GPIO.OUT)
GPIO.setup(PWMA, GPIO.OUT)
GPIO.setup(PWMB, GPIO.OUT)

pwma = GPIO.PWM(PWMA, 10000)            # Configure PWM, the motor drive frequency is about 10k best
pwmb = GPIO.PWM(PWMB, 10000)
pwma.start(0)
pwmb.start(0)
##############################################################################
###################################参数定义####################################
motor_slow = 10                                 # Car speed
motor_mid = 30
motor_fast = 50

RIGHT_MOTOR_PARAM = 1.2                # Define the operating coefficients of the left and right motors
                                       # separately so that the two motors maintain the same speed
LEFT_MOTOR_PARAM  = 0.67               # less than 1
##############################################################################


def motor_right(degree1):                  # Turn right, the parameter is the angle of rotation
    dc1 = degree1/3                         # Adjust the motor speed difference,
    dc2 = degree1/10                        # Used to adjust the turning radius
    pwma.ChangeDutyCycle(dc1)
    pwmb.ChangeDutyCycle(dc2)


def motor_left(degree2):                   # Turn left, the parameter is the angle of rotation
    dc1 = degree2/10                         # Adjust the motor speed difference,
    dc2 = degree2/3                         # Used to adjust the turning radius
    pwma.ChangeDutyCycle(dc1)
    pwmb.ChangeDutyCycle(dc2)


def motor_drive(degree):                   # Used to drive the car straight
    dc1 = degree*RIGHT_MOTOR_PARAM                     # Adjust the motor speed difference,
    dc2 = degree*LEFT_MOTOR_PARAM                      # Used to adjust the turning radius
    pwma.ChangeDutyCycle(dc1)
    pwmb.ChangeDutyCycle(dc2)


def motor_start():                          # 配置IO口，使两个电机正转，小车直行
    GPIO.output(INT1, GPIO.LOW)
    GPIO.output(INT2, GPIO.HIGH)
    GPIO.output(INT3, GPIO.LOW)
    GPIO.output(INT4, GPIO.HIGH)
    motor_drive(20)   # 启动速度：20
    
    
def motor_back():                         
    # Configure the IO port so that the two motors rotate forward and the car goes straight.
    GPIO.output(INT1, GPIO.HIGH)
    GPIO.output(INT2, GPIO.LOW)
    GPIO.output(INT3, GPIO.HIGH)
    GPIO.output(INT4, GPIO.LOW)
    motor_drive(15)   # Starting speed: 10


def motor_brake():                      # Brake
    motor_drive(0)


def motor_neutral():                    # 自由停止，适用于转速很快时缓慢停下
    GPIO.output(INT1, GPIO.LOW)
    GPIO.output(INT2, GPIO.LOW)
    GPIO.output(INT3, GPIO.LOW)
    GPIO.output(INT4, GPIO.LOW)
    motor_drive(0)


def motor_angle(degree):              
    # Free stop, suitable for slow stop when the speed is very fast
    if degree == -45:                    
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(1.2)
    elif degree == 45:                  
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(1.1)
    elif degree == -60:            
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(1)
    elif degree == -90:               
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(1.5)
    elif degree == 90:             
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(2)
    elif degree == -2:          
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(0.1)
    elif degree == 2:               
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(0.1)
    elif degree == -5:  
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(0.2)
    elif degree == 5:  
        pwma.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
        time.sleep(0.15)
    elif degree == 91: 
        pwma.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
        time.sleep(2.4)
    else:               
        motor_drive(0)
        time.sleep(0.15)
    return


def motor_corner(degree):
    # Wall turn
    if degree == -2:  
        GPIO.output(INT1, GPIO.HIGH)
        GPIO.output(INT2, GPIO.LOW)
        GPIO.output(INT3, GPIO.LOW)
        GPIO.output(INT4, GPIO.HIGH)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.1)
    elif degree == 2:  
        GPIO.output(INT1, GPIO.LOW)
        GPIO.output(INT2, GPIO.HIGH)
        GPIO.output(INT3, GPIO.HIGH)
        GPIO.output(INT4, GPIO.LOW)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.1)
    elif degree == -5:  
        GPIO.output(INT1, GPIO.HIGH)
        GPIO.output(INT2, GPIO.LOW)
        GPIO.output(INT3, GPIO.LOW)
        GPIO.output(INT4, GPIO.HIGH)
        pwma.ChangeDutyCycle(30)
        pwmb.ChangeDutyCycle(30)
        time.sleep(0.2)
    elif degree == 5:  
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

