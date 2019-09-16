# Coding = utf-8
# Written by You Hao, 2019
import os
import serial

class MBED:
    #Initialize serial communication with mbed
    def __init__(self):
        #Create a file object containing the mbed address
        with os.popen('python -m serial.tools.list_ports') as read_serial_port:
            #Extract the first 12 characters of the file, this is the address of the mbed under the linux system
            serial_port = read_serial_port.read()[0:12]
        
        #Establish serial communication object mbed
        mbed = serial.Serial(serial_port,9600,timeout=0.5)
        self.mbed = mbed
        #Open serial port
        if mbed.isOpen() != 1:
            mbed.open()
    
    def inBuffer(self):# Read buffer bytes
        return self.mbed.inWaiting()

    # Receive data from the buffer, receive error returns 1, successfully return to the receive field
    # Note: After calling this function to receive data, the buffer of serial communication is cleared.
    def rx(self):
        if self.mbed.inWaiting()==0:
            return 1
        else:
            read_buffer = self.mbed.read(self.mbed.inWaiting()) # Read all bytes of the buffer
            read_buffer = read_buffer.decode(encoding="UTF-8",errors="ignore")
            print('MBED return: %s'%read_buffer)
            return read_buffer
        
    # Transfer data to mbed
    def tx(self,send_buffer):
        send_buffer = str(send_buffer)
        # Use gbk encoding for the string to be transmitted
        send_buffer = send_buffer.encode(encoding="gbk",errors="ignore")
        # send
        self.mbed.write(b'%s\n'%send_buffer)