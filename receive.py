import serial
import time
import os

def get_time(void):
    return(time.strftime('%H:%M:%S',time.localtime(time.time())))

with os.popen('python -m serial.tools.list_ports') as read_serial_port:
    serial_port = read_serial_port.read()[0:12]

mbed = serial.Serial(serial_port,9600,timeout=0.5)

if mbed.isOpen() != 1:
    mbed.open()

while True:
    if mbed.inWaiting() > 0:
        read_buffer = mbed.read(mbed1.inWaiting())
        if read_buffer == b'T':
            CST_time = get_time().encode(encoding="gbk",errors="ignore")
            mbed.write(b'%s\n'%CST_time)
    time.sleep(0.5)

#python ~/Desktop/receive.py
