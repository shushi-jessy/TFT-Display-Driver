import tkinter
import tkinter.filedialog
import serial
import serial.tools.list_ports
import PIL.Image
import time
import os
import threading



img     = None
ser     = None
timeout_flag = 0
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
                            timeout=0.01)
        btn_refresh["state"]=tkinter.DISABLED
        btn_connect.config(text= "Disconnect",command=SerialDisconnect)
        ent_baudrate["state"]=tkinter.DISABLED
        opm_serial["state"]=tkinter.DISABLED
        lab_serialstate.config(text="Connected",bg="green")
        global img
        if img!= None:
            bnt_transfer["state"]=tkinter.NORMAL
        bnt_browse["state"]=tkinter.NORMAL
        bnt_read["state"]=tkinter.NORMAL
        root.title("Serial Assistant - Connected")
    except:
        lab_serialstate.config(text="Error",bg="yellow")


def SerialDisconnect():
    global ser
    ser.close()
    btn_refresh["state"]=tkinter.NORMAL
    btn_connect.config(text= "Connect",command=SerialConnect)
    ent_baudrate["state"]=tkinter.NORMAL
    opm_serial["state"]=tkinter.NORMAL
    lab_serialstate.config(text="NOT Connected",bg="red")
    bnt_browse["state"]=tkinter.DISABLED
    bnt_transfer["state"]=tkinter.DISABLED
    bnt_read["state"]=tkinter.DISABLED
    lab_process2["text"] = "0%"
    root.title("Serial Assistant - Disconnected")

def ReadData():
    global ser
    while ser.isOpen():
        if ser.in_waiting>0:
            print(ser.read(ser.in_waiting).decode("utf-8")) 
         
def SerialWriteFile(data_size,package_size):
    bnt_browse["state"]=tkinter.DISABLED
    bnt_transfer["state"]=tkinter.DISABLED
    lab_process1["text"]= data_size
    lab_process2["text"]= 0;
    assert(int(data_size%package_size) == 0) #check input arguments
    package_number = int(data_size/package_size)
    global ser
    global timeout_flag
    #x1 = threading.Thread(target=timeout)
    #x1.start()
    #x2 = threading.Thread(target=ReadData)
    #x2.start() 
    ser.flush() # Cleaname)
    file = open("data.txt","rb")
    buffer=[0]*2304
    data=[0]*2304
    while package_number>-1:
        ser.reset_output_buffer()
        ser.reset_input_buffer()
        timeout_flag = 0
        data = file.read(2304)
        ser.reset_input_buffer()
        ser.write(data)
        while ser.out_waiting!=0:
            pass
        ser.reset_output_buffer()
        time.sleep(0.1)
            #for k in range(960):
                #if buffer[k] != data[k]:
                    #print("data",k,"is different: buf=",buffer[k],"data=",data[k])
                    #print("Error exit in package",package_number,", now resend this one.")
                    #print("package lost")
                    #ser.reset_output_buffer()
                    #ser.reset_input_buffer()
                    #break
            #print("pakacge",package_number+1,"is checked!")
        package_number -= 1
            
            
        
        lab_process1["text"] = "bytes left:\t{0}".format(package_size*package_number)
        lab_process2["text"] = "{0:.1f}%".format(100-(package_size*(package_number))/data_size*100)
        lab_process1.update()
        lab_process2.update()
       
        
    bnt_browse["state"]=tkinter.NORMAL
    bnt_transfer["state"]=tkinter.NORMAL
    file.close()
        
    #except:
        #lab_serialstate.config(text="Error",bg="yellow")
        #SerialDisconnect()
    file.close()



def GetImageData(img):
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


def SerialTransfer():
    global img
    global ser
    GetImageData(img)
    SerialWriteFile(int(320*480*1.5*2),2304)

    #except:
        #SerialDisconnect()
        #lab_serialstate.config(text="Transfer Erro",bg="yellow")
            

    #os.remove("data.txt")
        

                                                   ##


 
# Monitor Winodw                            Monitor Winodw                       Monitor Winodw 
###############################################################################################
class Monitor:                                                                               ##
    def __init__(self,state_btn):                                                            ##
        self.state_btn = state_btn                                                           ##
        self.state_btn["state"]=tkinter.DISABLED                                             ##
        self.monitor = tkinter.Tk()                                                          ##
        self.monitor.geometry("500x500")                                                     ##
        self.monitor.resizable(width=False,height=False)                                     ##
        self.monitor.protocol("WM_DELETE_WINDOW",self.MonitorClose)                          ##
        self.lab_monitor = tkinter.Label(self.monitor,text="RX data mode:")                  ##
        self.lab_monitor.place(relx=0.025,rely=0,relwidth=0.2,relheigh=0.1)                  ##
        self.cbx_ascii = tkinter.IntVar()                                                    ##
        self.cbx_ascii.set(1)                                                                ##
        self.cbx_monitor = tkinter.Checkbutton(self.monitor,text='ASCII',                    ##
                                               variable=self.cbx_ascii,                      ##
                                               onvalue=1,                                    ##
                                               offvalue=0)                                   ##
        self.cbx_monitor.place(relx=0.225,rely=0,relwidth=0.15,relheigh=0.1)                 ##
        self.cbx_monitor["state"]=tkinter.DISABLED                                           ##
        self.txt_monitor=tkinter.Text(self.monitor)                                          ##
        self.txt_monitor.insert(tkinter.INSERT,                                              ##
                                "Connected, watting for information...\n")                   ##
        self.txt_monitor.place(relx=0.025,rely=0.1,relwidth=0.95,relheigh=0.8)               ##
        self.btn_del = tkinter.Button(self.monitor,                                          ##
                                      text="Clear",                                          ##
                                      command=self.DataContentClear)                         ##
        self.btn_del.place(relx=0.75,rely=0.03,relwidth=0.15,relheigh=0.06)                  ##
        self.monitor_thread = threading.Thread(target=self.ThreadFunction)                   ##
        self.monitor_thread.start()                                                          ##
        self.monitor.mainloop()                                                              ##
    def ThreadFunction(self):                                                                ##
        global ser                                                                           ##
        try:                                                                                 ##
            while True:                                                                      ##
                if ser.in_waiting>0:                                                         ##
                    self.buf = ser.read(ser.in_waiting).decode("utf-8")                      ##
                    self.txt_monitor.insert(tkinter.INSERT,self.buf)                         ##
                    self.txt_monitor.see("end")                                              ##
                time.sleep(2)                                                                ##
        except:                                                                              ##
                pass                                                                         ##
    def DataContentClear(self):                                                              ##
        self.txt_monitor.delete('1.0', tkinter.END)                                          ##
        self.txt_monitor.insert(tkinter.INSERT,"Connected, watting for information...\n")    ##
    def MonitorClose(self):                                                                  ##
        self.txt_monitor.delete('1.0', tkinter.END)                                          ##
        self.txt_monitor.insert(tkinter.INSERT,"Connection lost!")                           ##
        self.monitor.update()                                                                ##
        time.sleep(0.5)                                                                      ##
        self.monitor.destroy()                                                               ##
        del self                                                                             ##
        SerialDisconnect()                                                                   ##
###############################################################################################
# Monitor Winodw                            Monitor Winodw                       Monitor Winodw
 

root = tkinter.Tk()
root.geometry("500x200")
root.resizable(width=False,height=False)
root.title("Serial Assistant - Not Connected")
#root.withdraw()
# Serial interface:
lab_serial = tkinter.Label(root,text="COM port")
lab_serial.place(relx=0,rely=0.1,relwidth=0.2,relheigh=0.1)
lab_baudrate = tkinter.Label(root,text="Baud Rate")
lab_baudrate.place(relx=0,rely=0.3,relwidth=0.2,relheigh=0.1)
lab_baudrateunit = tkinter.Label(root,text="bits/s")
lab_baudrateunit.place(relx=0.35,rely=0.3,relwidth=0.2,relheigh=0.1)
lab_serialstate = tkinter.Label(root,text="NOT connected",bg="red")
lab_serialstate.place(relx=0.5,rely=0.3,relwidth=0.2,relheigh=0.1)
btn_refresh = tkinter.Button(root,text="Refresh",command=SerialFindPort)
btn_refresh.place(relx=0.75,rely=0.075,relwidth=0.2,relheigh=0.15)
btn_connect = tkinter.Button(root,text="Connect",command=SerialConnect)
btn_connect.place(relx=0.75,rely=0.275,relwidth=0.2,relheigh=0.15)
bnt_read = tkinter.Button(root,text="Start RX",command=lambda: Monitor(bnt_read),state=tkinter.DISABLED)
bnt_read.place(relx=0.75,rely=0.85,relwidth=0.2,relheigh=0.1)

# File interface:
lab_browse = tkinter.Label(root,text="File")
lab_browse.place(relx=0,rely=0.5,relwidth=0.2,relheigh=0.1)
lab_file = tkinter.Label(root,text="Please browse an image file")
lab_file.place(relx=0.2,rely=0.5,relwidth=0.5,relheigh=0.1)
lab_process = tkinter.Label(root,text="Process")
lab_process.place(relx=0,rely=0.7,relwidth=0.2,relheigh=0.1)
lab_process1 = tkinter.Label(root,text="bytes left:\t0")
lab_process1.place(relx=0.2,rely=0.7,relwidth=0.5,relheigh=0.1)
lab_process2 = tkinter.Label(root,text= "0%")
lab_process2.place(relx=0.2,rely=0.8,relwidth=0.5,relheigh=0.1)
bnt_browse = tkinter.Button(root,text="Browse",command=BrowseFile,state=tkinter.DISABLED)
bnt_browse.place(relx=0.75,rely=0.475,relwidth=0.2,relheigh=0.15)
bnt_transfer = tkinter.Button(root,text="Transfer",command=SerialTransfer,state=tkinter.DISABLED)
bnt_transfer.place(relx=0.75,rely=0.675,relwidth=0.2,relheigh=0.15)    


    

# Exit:
#bnt_exit = tkinter.Button(root,text="exit",command=App_exit)
#bnt_exit.place(relx=0.75,rely=0.825,relwidth=0.2,relheigh=0.15)



 



if __name__ == "__main__" :
    SerialFindPort() 
    root.mainloop()
    #img = PIL.Image.open(img_filename)
    
