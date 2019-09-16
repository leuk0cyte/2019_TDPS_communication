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
        GPIO.output(tri, GPIO.HIGH)           # Trigger signal
        time.sleep(0.000015)                # Keep above 10us (I choose 15us)
        GPIO.output(tri, GPIO.LOW)
        t_all = time.time()
        while not GPIO.input(echo):
            if (time.time() - t_all) > 0.5:
                n = 1
                break
        t1 = time.time()                    # Timed when the high level is found
        while GPIO.input(echo):
                pass
        t2 = time.time()                    # High level end stop timing
        distan.append((t2-t1)*340/2)        # Return distance in meters
        if n == 1:
            distan[a] = 0
            m = m + 1
        total_length = distan[a] + total_length
        time.sleep(0.001)
    max_list = max(distan)
    avg_length = (total_length - max_list)/(9 - m)
    return avg_length

def corner_adjust(tolerance):
    yeah = 0
    while yeah == 0:
        distance1 = checkdist(1)
        distance2 = checkdist(2)
        print('distance1 = %0.3f m' % distance1)
        print('distance2 = %0.3f m' % distance2)
        if (distance1 - distance2) > tolerance:
            if (distance1 - distance2) > (tolerance * 2):
                motor_corner(-5)
                motor_brake()
                time.sleep(0.5)
            else:
                motor_corner(-2)
                motor_brake()
                time.sleep(0.5)
        elif (distance2 - distance1) > tolerance:
            if (distance2 - distance1) > tolerance * 2:
                motor_corner(5)
                motor_brake()
                time.sleep(0.5)
            else:
                motor_corner(2)
                motor_brake()
                time.sleep(0.5)
        else:
            yeah = 1