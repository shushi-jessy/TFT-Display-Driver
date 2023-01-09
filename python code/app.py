import tkinter
import tkinter.filedialog
import serial
import serial.tools.list_ports
import PIL.Image
import time
import os

img     = None

def RGB888_to_RGB666(rgb888):
    rgb666=[0,0,0]
    rgb666[0] = int(rgb888[0]  / 255 * 63)
    rgb666[1] = int(rgb888[1]  / 255 * 63)
    rgb666[2] = int(rgb888[2]  / 255 * 63)
    return rgb666

def BrowseFile():
    try:
        global img
        img = PIL.Image.open(tkinter.filedialog.askopenfilename(initialdir="/",title="select a File",filetypes=(("JP(E)G files","*.jpg *.jpeg"),("PGN files","*.png"))))
        filename = img.filename
        bnt_transfer["state"]=tkinter.NORMAL
    except:
        filename = "Please browse an image file" 
    global lab_file
    lab_file["text"]=filename

def App_exit():
    root.destroy()

def SerialFindPort():
    global port_var
    global ent_baudrate
    global opm_serial
    ports = serial.tools.list_ports.comports()
    port_var = tkinter.StringVar()
    port_opt = ["Please select a port"]
    port_var.set(port_opt[0])
    for p in ports:
        port_opt.append(p[0])
    port_opt.remove("Please select a port")
    opm_serial = tkinter.OptionMenu(root,port_var,*port_opt)
    opm_serial.place(relx=0.2,rely=0.1,relwidth=0.5,relheigh=0.1)
    ent_baudrate = tkinter.Entry(root)
    ent_baudrate.insert(0,"2000000")
    ent_baudrate.place(relx=0.2,rely=0.275,relwidth=0.2,relheigh=0.15)

def SerialConnect():
    try:
        global ser
        ser = serial.Serial(port= port_var.get(),
                            baudrate=ent_baudrate.get(),
                            timeout=1)
        bnt_serial["state"]=tkinter.DISABLED
        bnt_connect.config(text= "Disconnect",command=SerialDisconnect)
        ent_baudrate["state"]=tkinter.DISABLED
        opm_serial["state"]=tkinter.DISABLED
        lab_serialstate.config(text="Connected",bg="green")
        buf = ""
    except:
        lab_serialstate.config(text="Error",bg="yellow")


def SerialDisconnect():
    global ser
    ser.close()
    bnt_serial["state"]=tkinter.NORMAL
    bnt_connect.config(text= "Connect",command=SerialConnect)
    ent_baudrate["state"]=tkinter.NORMAL
    opm_serial["state"]=tkinter.NORMAL
    lab_serialstate.config(text="NOT Connected",bg="red")
    bnt_browse["state"]=tkinter.NORMAL
    bnt_transfer["state"]=tkinter.NORMAL
    lab_process2["text"] = "0%"


def SerialWrite():
    bnt_browse["state"]=tkinter.DISABLED
    bnt_transfer["state"]=tkinter.DISABLED
    lab_process2["text"]= 0
    global ser
    filesize = 320*480*1.5*2
    byte_sent = 0;
    byte_left = filesize;
    root.update()
    global img
    img = img.convert('RGB')
    img = img.resize((320, 480))
    try:
        os.remove("data.txt")
    except:
        pass
    file = open("data.txt","x")
    file = open("data.txt","wb")
    buf = [0]*6
    for i in range(0,320,1):
        for j in range(0,480,2):
            RGB666_pix1 = RGB888_to_RGB666(img.getpixel((i,j)))
            RGB666_pix2 = RGB888_to_RGB666(img.getpixel((i,j+1)))
            buf[0] = (RGB666_pix1[0]<<2)
            buf[1] = (RGB666_pix1[1]<<2)
            buf[2] = (RGB666_pix1[2]<<2)
            buf[3] = (RGB666_pix2[0]<<2)
            buf[4] = (RGB666_pix2[1]<<2)
            buf[5] = (RGB666_pix2[2]<<2)
            file.write(bytes.fromhex("{0:02X}{1:02X}".format(buf[0],buf[1])))
            file.write(bytes.fromhex("{0:02X}{1:02X}".format(buf[2],buf[3])))
            file.write(bytes.fromhex("{0:02X}{1:02X}".format(buf[4],buf[5])))
    file.close()
    try:
        ser.flush() # Clean
        file = open("data.txt","rb")
        for i in range(200):
                ser.reset_output_buffer()
                ser.write(file.read(2304))
                while ser.out_waiting!=0:
                    pass
                while ser.in_waiting>0:
                    print(ser.read(ser.in_waiting).decode("utf-8"))
                percent = (i+1)/2
                lab_process2["text"] = "{0:.1f}%".format(percent)
                lab_process2.update()
                time.sleep(0.2)
        bnt_browse["state"]=tkinter.NORMAL
        bnt_transfer["state"]=tkinter.NORMAL
        file.close()
    #os.remove("data.txt")
        
    except:
        lab_serialstate.config(text="Error",bg="yellow")
        bnt_browse["state"]=tkinter.NORMAL
        bnt_transfer["state"]=tkinter.NORMAL
        lab_process2["text"] = "0%"
    

def SerialRead():
    if ser.in_waiting>0:
        print(ser.read(ser.in_waiting).decode("utf-8"))
    
root = tkinter.Tk()
root.geometry("500x200")
root.resizable(width=False,height=False)

# Serial interface:
lab_serial = tkinter.Label(root,text="COM port")
lab_serial.place(relx=0,rely=0.1,relwidth=0.2,relheigh=0.1)
lab_baudrate = tkinter.Label(root,text="Baud Rate")
lab_baudrate.place(relx=0,rely=0.3,relwidth=0.2,relheigh=0.1)
lab_baudrateunit = tkinter.Label(root,text="bits/s")
lab_baudrateunit.place(relx=0.35,rely=0.3,relwidth=0.2,relheigh=0.1)
lab_serialstate = tkinter.Label(root,text="NOT connected",bg="red")
lab_serialstate.place(relx=0.5,rely=0.3,relwidth=0.2,relheigh=0.1)
bnt_serial = tkinter.Button(root,text="Refresh",command=SerialFindPort)
bnt_serial.place(relx=0.75,rely=0.075,relwidth=0.2,relheigh=0.15)
bnt_connect = tkinter.Button(root,text="Connect",command=SerialConnect)
bnt_connect.place(relx=0.75,rely=0.275,relwidth=0.2,relheigh=0.15)
bnt_read = tkinter.Button(root,text="Read",command=SerialRead)
bnt_read.place(relx=0.75,rely=0.85,relwidth=0.2,relheigh=0.1)

# File interface:
lab_browse = tkinter.Label(root,text="File")
lab_browse.place(relx=0,rely=0.5,relwidth=0.2,relheigh=0.1)
lab_file = tkinter.Label(root,text="Please browse an image file")
lab_file.place(relx=0.2,rely=0.5,relwidth=0.5,relheigh=0.1)
lab_process1 = tkinter.Label(root,text="Process")
lab_process1.place(relx=0,rely=0.7,relwidth=0.2,relheigh=0.1)
lab_process2 = tkinter.Label(root,text= "0%")
lab_process2.place(relx=0.2,rely=0.7,relwidth=0.5,relheigh=0.2)
bnt_browse = tkinter.Button(root,text="Browse",command=BrowseFile)
bnt_browse.place(relx=0.75,rely=0.475,relwidth=0.2,relheigh=0.15)
bnt_transfer = tkinter.Button(root,text="Transfer",command=SerialWrite,state=tkinter.DISABLED)
bnt_transfer.place(relx=0.75,rely=0.675,relwidth=0.2,relheigh=0.15)

# Exit:
#bnt_exit = tkinter.Button(root,text="exit",command=App_exit)
#bnt_exit.place(relx=0.75,rely=0.825,relwidth=0.2,relheigh=0.15)



 



if __name__ == "__main__" :
    SerialFindPort()
    root.mainloop()
    #img = PIL.Image.open(img_filename)
    
