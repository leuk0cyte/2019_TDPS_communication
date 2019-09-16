from motor_control import *
GPIO.setup(3, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(17, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(27, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(2, GPIO.IN)
GPIO.setup(4, GPIO.IN)
GPIO.setup(22, GPIO.IN)

def checkdist(mode):
    if mode == 0:
        tri = 3
        echo = 2
    elif mode == 1:
        tri = 17
        echo = 4
    elif mode == 2:
        tri = 27
        echo = 22
    else:
        tri = 3
        echo = 2
    total_length = 0
    distan = []
    for a in range(10):
        m = 0
        n = 0
        GPIO.output(tri, GPIO.HIGH)           # 发出触发信号
        time.sleep(0.000015)                # 保持10us以上（我选择15us）
        GPIO.output(tri, GPIO.LOW)
        t_all = time.time()
        while not GPIO.input(echo):
            if (time.time() - t_all) > 0.5:
                n = 1
                break
        t1 = time.time()                    # 发现高电平时开时计时
        while GPIO.input(echo):
                pass
        t2 = time.time()                    # 高电平结束停止计时
        distan.append((t2-t1)*340/2)        # 返回距离，单位为米
        if n == 1:
            distan[a] = 0
            m = m + 1
        total_length = distan[a] + total_length
        time.sleep(0.001)
    max_list = max(distan)
    avg_length = (total_length - max_list)/(9 - m)
    return avg_length

