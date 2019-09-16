import RPi.GPIO as GPIO
import time

# 先附上树莓派3B+IO口网址：http://shumeipai.nxez.com/raspberry-pi-pins-version-40
# 树莓派PWM设定的网址：https://www.jianshu.com/p/4e9e0ecfa6aa


# 定义引脚：
INT1 = 21                            # 对应树莓派3b+的引脚为：40,38,37,35，36,33
INT2 = 20
INT3 = 26
INT4 = 19
PWMA = 16
PWMB = 13
aphase1 = 23                         # 对应树莓派3b+的引脚为：16,18,29,31
bphase1 = 24
aphase2 = 5
bphase2 = 6

# 测量计算参数
PPR = 341.2                                      # 霍尔编码器灵敏度
Precision = 10                                   # 每10次精确度测一次数
diameter_wheel = 0.04                            # 单位   米/m
distance_per_circle = diameter_wheel*2*3.14159
distance = (distance_per_circle/PPR)*Precision

# PID计算参数
KP = 5
KI = 0.2
KD = 0
l_Last_Bias = 0
l_int_bias = 0
r_Last_Bias = 0
r_int_bias = 0

# 初始化变量值
r_degree = 0
l_degree = 0
right_speed = 0
left_speed = 0
r_counter = 0      # 右轮脉冲初值
l_counter = 0      # 左轮脉冲初值

# 测试pid  print（）使用 赋予初值
actual_r_dutycycle = 0
actual_l_dutycycle = 0
r_pwm = 0
l_pwm = 0


# 设置GPIO状态
GPIO.setmode(GPIO.BCM)                 # 使用树莓派BCM引脚排列，而不是board
GPIO.setwarnings(False)                # 消除程序报错警告
GPIO.setup(INT1, GPIO.OUT)             # 配置IO口功能
GPIO.setup(INT2, GPIO.OUT)
GPIO.setup(INT3, GPIO.OUT)
GPIO.setup(INT4, GPIO.OUT)
GPIO.setup(PWMA, GPIO.OUT)
GPIO.setup(PWMB, GPIO.OUT)
GPIO.setup(aphase1, GPIO.IN)
GPIO.setup(bphase1, GPIO.IN)
GPIO.setup(aphase2, GPIO.IN)
GPIO.setup(bphase2, GPIO.IN)

pwma = GPIO.PWM(PWMA, 10000)            # 配置PWM，电机驱动频率大约10k最佳
pwmb = GPIO.PWM(PWMB, 10000)
pwma.start(0)
pwmb.start(0)
#    question：记得要封装在不同文件中


def right_counter(a_phase1):                                             # 边缘检测回调函数
    global r_counter, r_duration, right_speed, timer1, r_duration        # 设置为全局变量
    if GPIO.event_detected(a_phase1):                                    # 检测到一个脉冲则脉冲数加1
        r_counter = r_counter+1
        if r_counter == 10:
            stop_time1 = time.time()
            r_duration = stop_time1 - timer1
            right_speed = (distance / r_duration) + 0.00001
            timer1 = time.time()
            r_counter = 0


def left_counter(a_phase2):                                               # 边缘检测回调函数
    global l_counter, l_duration, left_speed, timer2, l_duration          # 设置为全局变量
    if GPIO.event_detected(a_phase2):                                     # 检测到一个脉冲则脉冲数加1
        l_counter = l_counter + 1
        if l_counter == 10:
            stop_time2 = time.time()
            l_duration = stop_time2 - timer2
            left_speed = (distance / l_duration) + 0.00001
            timer2 = time.time()
            l_counter = 0


def left_pid_calculation(leftspeed, theory_left_dc):                      # 增量式
    global actual_l_dutycycle, l_Last_Bias, l_int_bias, l_pwm
    actual_l_dutycycle = 72.316 * leftspeed - 10.274                      # excel进行拟合                       question：拟合公式修改
    if actual_l_dutycycle > 100:                                          # 限制占空比
        actual_l_dutycycle = 100
    elif actual_l_dutycycle < 0:
        actual_l_dutycycle = 0
    l_bias = theory_left_dc - actual_l_dutycycle                          # 求出速度偏差，由测量值减去目标值。
    l_int_bias += l_bias                                                  # 求出偏差的积分                      question：克服积分饱和，打算用积分限幅法（幅值需要实际测量）
    l_pwm_via = KP * l_bias + KI * l_int_bias + KD * (l_bias - l_Last_Bias)    # 增量式PID控制器                question：修改P,I,D参数（28到30行）
    l_Last_Bias = l_bias                                                  # 保存上一次偏差
    l_pwm = actual_l_dutycycle + l_pwm_via
    if l_pwm > 90:                                                        # 限制占空比
        l_pwm = 90
    elif l_pwm < 10:
        l_pwm = 10
    return l_pwm        # 增量输出，返回调整后的pwm(速度)值


def right_pid_calculation(rightspeed, theory_right_dc):                   # 增量式
    global actual_r_dutycycle, r_Last_Bias, r_int_bias, r_pwm
    actual_r_dutycycle = 72.316 * rightspeed - 10.274                     # excel进行拟合                       question：拟合公式修改
    if actual_r_dutycycle > 100:                                          # 限制占空比
        actual_r_dutycycle = 100
    elif actual_r_dutycycle < 0:
        actual_r_dutycycle = 0
    r_bias = theory_right_dc - actual_r_dutycycle                         # 求出速度偏差，由测量值减去目标值。
    r_int_bias += r_bias                                                  # 求出偏差的积分                      question：克服积分饱和，打算用积分限幅法（幅值需要实际测量）
    r_pwm_via = KP * r_bias + KI * r_int_bias + KD * (r_bias - r_Last_Bias)    # 增量式PID控制器                question：修改P,I,D参数（28到30行）
    r_Last_Bias = r_bias                                                  # 保存上一次偏差
    r_pwm = actual_r_dutycycle + r_pwm_via
    if r_pwm > 90:                                                        # 限制占空比
        r_pwm = 90
    elif r_pwm < 10:
        r_pwm = 10
    return r_pwm       # 增量输出，返回调整后的pwm(速度)值


def start():                            # 配置IO口，使两个电机正转，小车直行
    GPIO.output(INT1, GPIO.LOW)
    GPIO.output(INT2, GPIO.HIGH)
    GPIO.output(INT3, GPIO.LOW)
    GPIO.output(INT4, GPIO.HIGH)
    pwma.ChangeDutyCycle(20)            # 占空比暂时调整为20（速度： 20 单位 ）                                  question：可能要对初始速度进行调整
    pwmb.ChangeDutyCycle(20)
    return


def straight(speed):
    global right_speed, left_speed
    cite_right_pwm = right_pid_calculation(right_speed, speed)
    cite_left_pwm = left_pid_calculation(left_speed, speed)
    pwma.ChangeDutyCycle(cite_left_pwm)
    pwmb.ChangeDutyCycle(cite_right_pwm)
    return


def right(degree):                      # 右转，参数为转的角度
    global right_speed, left_speed
    l_pwm_need = degree / 90            # 后面修改参数                                                          question：修改公式
    r_pwm_need = degree * 5             # 后面修改参数                                                          question：修改公式
    cite_right_pwm = right_pid_calculation(right_speed, r_pwm_need)
    cite_left_pwm = left_pid_calculation(left_speed, l_pwm_need)
    pwma.ChangeDutyCycle(cite_left_pwm)
    pwmb.ChangeDutyCycle(cite_right_pwm)
    return


def left(degree):
    global right_speed, left_speed
    l_pwm_need = degree * 5            # 后面修改参数                                                          question：修改公式
    r_pwm_need = degree / 90           # 后面修改参数                                                          question：修改公式
    cite_right_pwm = right_pid_calculation(right_speed, r_pwm_need)
    cite_left_pwm = left_pid_calculation(left_speed, l_pwm_need)
    pwma.ChangeDutyCycle(cite_left_pwm)
    pwmb.ChangeDutyCycle(cite_right_pwm)
    return


def brakedown():                        # 急刹车
    pwma.ChangeDutyCycle(0)
    pwmb.ChangeDutyCycle(0)
    return


def stop_freely():                      # 自由停止，适用于转速很快时缓慢停下
    GPIO.output(INT1, GPIO.LOW)
    GPIO.output(INT2, GPIO.LOW)
    GPIO.output(INT3, GPIO.LOW)
    GPIO.output(INT4, GPIO.LOW)
    pwma.ChangeDutyCycle(20)
    pwmb.ChangeDutyCycle(20)
    return


GPIO.add_event_detect(aphase1, GPIO.RISING, callback=right_counter)   # 在引脚上添加上升临界值检测再回调
GPIO.add_event_detect(aphase2, GPIO.RISING, callback=left_counter)    # 在引脚上添加上升临界值检测再回调
timer1 = time.time()   # 单位为秒
timer2 = timer1
start()
time.sleep(3)
while 1:               # 主函数，可修改                                                                         question：主函数修改
    right(r_degree)
    print('实际输出pwm', actual_r_dutycycle)
    print('返回调节pwm', r_pwm)
    time.sleep(0.05)
