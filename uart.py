from serial import *

ser = Serial(port='/dev/cu.usbserial-11220',
                        baudrate=2000000,
                        bytesize=EIGHTBITS,
                        parity=PARITY_NONE,
                        stopbits=STOPBITS_ONE,
                        timeout=1)

flag = 0
buf = ""
while True:
    while ser.in_waiting>0:
        buf = buf + ser.read(1).decode("utf-8")
    print(buf)
    buf = ""
    while ser.in_waiting==0:
        pass
