import tkinter as tk
import serial
import time
import MySQLdb

ser =serial.Serial('/dev/ttyACM0',9600)
ser.flushInput()

class ScoreBoard(tk.Frame):
    def __init__(self,master=None):
        super().__init__(master)
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.grid_rowconfigure(0,weight=1)
        self.grid_columnconfigure(0,weight=1)
  
        self.periodLabel =tk.Label(self,text="PERIOD")
        self.periodLabel.config(font=("Franklin Gothic Demi Cond",33))
        self.periodLabel.grid(row=0,column=1,sticky="WE",pady=(100,0))

        self.periodTime =tk.Label(self,text="00:00",fg="red")
        self.periodTime.config(font=("Digital-7",100),bg="#FFFFFF",width=5,anchor="n")
        self.periodTime.grid(row=1,column=1,sticky="WE",pady=(0,125),ipady=15)

        self.whiteLabel =tk.Label(self,text="WHITE")
        self.whiteLabel.config(font=("Franklin Gothic Demi Cond",33))
        self.whiteLabel.grid(row=3,column=0,sticky="WE")

        self.whiteScore =tk.Label(self,text="000",fg="red")
        self.whiteScore.config(font=("Digital-7",120),width=3,bg="#FFFFFF",anchor="n")
        self.whiteScore.grid(row=4,column=0,sticky="WE",padx=50,ipady=15)
        
        self.jamLabel =tk.Label(self,text="JAM")
        self.jamLabel.config(font=("Franklin Gothic Demi Cond",33))
        self.jamLabel.grid(row=3,column=1,sticky="WE")

        self.jamTime =tk.Label(self,text="00:00",fg="red")
        self.jamTime.config(font=("Digital-7",100),bg="#FFFFFF",width=5,anchor="n")
        self.jamTime.grid(row=4,column=1,sticky="WE",ipady=15)

        self.blackLabel =tk.Label(self,text="BLACK")
        self.blackLabel.config(font=("Franklin Gothic Demi Cond",33))
        self.blackLabel.grid(row=3,column=2,sticky="WE")

        self.blackScore =tk.Label(self,text="000",fg="red")
        self.blackScore.config(font=("Digital-7",120),width=3,bg="#FFFFFF",anchor="n")
        self.blackScore.grid(row=4,column=2,sticky="WE",padx=50,ipady=15)

        self.quit=tk.Button(self,text="QUIT",fg="red",command=root.destroy)
        self.quit.grid(row=5,column=1,pady=(60,0))

    def send_to_db(self,gameID,name,value):
        db = MySQLdb.connect(host="localhost",user="scorekeeper",passwd="derbs",db="derby")
        cursor = db.cursor()
        sql ="UPDATE gameTBL SET "+name+" = %s WHERE id = %s"
        data=(value,gameID)
        try:
            cursor.execute(sql,data)
        except (MySQLdb.Error,MySQLdb.Warning) as e:
            print(e)
        db.commit()
        db.close()
    
    def read_from_arduino(self):
        #while True:
        try:
            state=ser.readline().decode("utf-8")
           # print(state)
            if state[:1]=='W':
                value=state.split(':')[1].strip()
                self.whiteScore.configure(text=value)
                self.send_to_db(0,"whiteScore",value)
            if state[:1]=='B':
                value=state.split(':')[1].strip()
                self.blackScore.configure(text=value)
                self.send_to_db(0,"blackScore",value)
            if state[:1]=='J':
                value=state.split(':',1)[1].strip()
                self.jamTime.configure(text=value)
                self.send_to_db(0,"jamTime",value)
            if state[:1]=='P':
                value=state.split(':',1)[1].strip()
                self.periodTime.configure(text=value)
                self.send_to_db(0,"periodTime",value)
            if state[:1]=='F':
                ser.flushInput()
        except:
            pass
       # ser.flushInput()
        self.after(45,self.read_from_arduino)


root = tk.Tk()
root.overrideredirect(True)
root.geometry("{0}x{1}+0+0".format(root.winfo_screenwidth(),root.winfo_screenheight()))
app = ScoreBoard(master=root)
app.read_from_arduino()
app.mainloop()
