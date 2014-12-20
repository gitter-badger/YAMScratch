import os
import sys
import re
import Tkinter as tk

'''Keeps track of lines of code written in target directory
    Excludes newlines and comments from loc count (no cheating)

    First version is a command line tool
    After that I will fill in a GUI
'''

class DirReader(object):
    def __init__(self):
        pass

'''===============GUI CODE==============='''

class MainWindow(tk.Frame):

    def __init__(self,parent):
        tk.Frame.__init__(self,parent)

        self.window_width = 400
        self.window_height = 400
        self.canvas = tk.Canvas(width = self.window_width,height = self.window_height)
        self.canvas.pack(fill = "both",expand = True)
        self.canvas.bind("<1>",lambda event: self.canvas.focus_set())

if __name__ == "__main__":



    '''GUI PORTION
    root = tk.Tk()
    mw = MainWindow(root)
    mw.pack()

    root.mainloop()
    '''
