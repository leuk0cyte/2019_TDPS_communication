from joysticks import *
import time 
joysticks=JOYSTICKS()

while 1:
    for i in range(4):
        axis = joysticks.axis(i)
    time.sleep(1)
