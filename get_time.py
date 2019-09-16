#NAME       : get_time
#INTROCTION : A function to get 24-clock time like hh:mm:ss
#INPUT      : None
#OUTPUT     : 8-byte string
#PAKAGE RELY: time
import time
def get_time(void):
    return(time.strftime('%H:%M:%S',time.localtime(time.time())))
print(type(get_time(0)))