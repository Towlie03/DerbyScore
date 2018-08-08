import tkinter as tk
import serial
import time

ser =serial.Serial('/dev/ttyACM0',9600)

class ScoreBoard(tk.Frame):
    def __init__(self,master=None):
        super().__init__(master)
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.whiteScore =tk.Label(self,text="Test")
        self.whiteScore.config(font=("Courier",34))
        self.whiteScore.pack(side="left")
        self.blackScore =tk.Label(self,text="test")
        self.blackScore.config(font=("Courier",34))
        self.blackScore.pack(side="right")
        self.jamTime =tk.Label(self,text="test")
        self.jamTime.config(font=("Courier",34))
        self.jamTime.pack(side="bottom")
        self.periodTime =tk.Label(self,text="test")
        self.periodTime.config(font=("Courier",34))
        self.periodTime.pack(side="bottom")
        self.quit=tk.Button(self,text="QUIT",fg="red",command=root.destroy)
        self.quit.pack(side="bottom")

    def read_from_arduino(self):
        #while True:
        try:
            state=ser.readline().decode("utf-8")
            #print(state)
            if state[:1]=='W':
                self.whiteScore.configure(text="WHITE:"+state.split(':')[1])
            if state[:1]=='B':
                self.blackScore.configure(text="BLACK:"+state.split(':')[1])
            if state[:1]=='J':
                self.jamTime.configure(text="JAM:"+state.split(':',1)[1])
            if state[:1]=='P':
                self.periodTime.configure(text="PERIOD:"+state.split(':',1)[1])
        except:
            pass
        ser.flush()
        self.after(25,self.read_from_arduino)


root = tk.Tk()
root.overrideredirect(True)
root.geometry("{0}x{1}+0+0".format(root.winfo_screenwidth(),root.winfo_screenheight()))
app = ScoreBoard(master=root)
app.read_from_arduino()
app.mainloop()
