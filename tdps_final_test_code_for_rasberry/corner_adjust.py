from motor_control import *
from ultrasonic_navigation import *
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
if __name__=='__main__':
    corner_adjust(0.01)
    corner_adjust(0.01)
    corner_adjust(0.01)
    