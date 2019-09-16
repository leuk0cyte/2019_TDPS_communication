
import time
import os
def get_time():
    return(time.strftime('%H:%M:%S',time.localtime(time.time())))

CST_time = get_time().encode(encoding="gbk",errors="ignore")

for i in range(0,3):
    print('%s\n'%CST_time)
    pass
