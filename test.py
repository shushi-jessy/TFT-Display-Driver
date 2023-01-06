from tkinter import *
from PIL.Image import *
from tkinter.filedialog import *
from serial import *
import serial.tools.list_ports

def RGB888_to_RGB565(rgb888):
    rgb565=[0,0,0]
    rgb565[0] = int(rgb888[0]  / 255 * 31)
    rgb565[1] = int(rgb888[1]  / 255 * 63)
    rgb565[2] = int(rgb888[2]  / 255 * 31)
    return rgb565

def Serialize():
    buf=[0]*2
    global img_resized
    # bit:    15   14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
    # color:  R4   R3  R2  R1  R0  G5  G4  G3  G2  G1  G0  B4  B3  B2  B1  B0
    # color:  [------ Red ------]  [------- Green ------]  [----- Blue -----]
    buf=[0]*2
    ser = Serial(port='/dev/cu.usbserial-1110',
                        baudrate=2000000,
                        bytesize=EIGHTBITS,
                        parity=PARITY_NONE,
                        stopbits=STOPBITS_ONE,)
    ser.flush() #avoid noise
    for i in range(0,320,1):
        for j in range(0,480,1):
            rgb555 = RGB888_to_RGB565(img_resized.getpixel((i,j)))
            buf[0] = (rgb555[0]<<3) + ((rgb555[1]&0b00111000)>>3)
            buf[1] = ((rgb555[1]&0b00000111)<<5) + rgb555[2]
            if ser.out_waiting>0:
                ser.flush()  #avoid noise          
            ser.write(buf)
            while ser.out_waiting>0:
                pass
    global cp
    cp.destroy()
    
def BrowseFile():
    global img_filename
    img_filename = askopenfilename(initialdir="/",
                               title="select a File",
                               filetypes=(("JP(E)G files","*.jpg *.jpeg"),("PGN files","*.png")))
def SendData():
    global btn1
    btn1["state"] = DISABLED
    btn1["text"]="Sending data..."
    btn1.update
    Serialize
    
def ControlPannel():
    global cp
    cp = Tk()
    cp.geometry('400x100')
    cp.resizable(width=0, height=0)
    global img_filename
    img_filename = None
    global btn1
    btn1 = Button(cp,text="*** Click here to browse ***",command=BrowseFile)
    btn1.place(relwidth=1,relheight=1,relx=0,rely=0)
    #print("Please select an image file!")
    while img_filename == None:
        cp.update()
    #print("An image file has been selected:\n\t",img_filename)
    img = open(img_filename)
    img = img.convert('RGB')
    global img_resized
    img_resized = img.resize((320, 480))
    #print("Origin photo size:\t",img.size[0],"x",img.size[1])
    #print("Resized photo size:\t",img_resized.size[0],"x",img_resized.size[1])
    #print("---------------------------------------")
    btn1.config(text=img_filename+"\n** Click here to send data ***",command=Serialize)
    cp.update()
    #img_resized.show()
    cp.mainloop()


if __name__ == "__main__" :
    ports = serial.tools.list_ports.comports()
    for p in ports:
        print(p)
    ControlPannel()
    
