from serial import *
import time
ser = Serial(port='/dev/cu.usbserial-11220',
                        baudrate=2000000,
                        bytesize=EIGHTBITS,
                        parity=PARITY_NONE,
                        stopbits=STOPBITS_ONE,
                        timeout=1)

buf = ""
data=[12]*255
while True:
    while ser.in_waiting>0:
        buf = buf + ser.read(1).decode("utf-8")
    print(buf)
    if ser.out_waiting==0:
        ser.write(data)
    buf = ""
    while  ser.in_waiting==0:
        pass


