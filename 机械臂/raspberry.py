import serial
import time
import os


with os.popen('python -m serial.tools.list_ports') as read_serial_port:
    serial_port = read_serial_port.read()[0:12]

mbed=serial.Serial(serial_port,9600,timeout=0.5)

if mbed.isOpen() != 1:
    mbed.open()

while 1:
    mode_code = 2

    mbed.write(b'%i\n'%mode_code)

    while mbed.inWaiting()==0:
        time.sleep(0.5)


    read_buffer = mbed.read(mbed.inWaiting())
    print(read_buffer)

