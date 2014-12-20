import os
import sys
import re
import sqlite3

import Tkinter as tk

'''Keeps track of lines of code written in target directory
    Excludes newlines and comments from loc count (no cheating)

    First version is a command line tool
    After that I will fill in a GUI
'''

class FileReader(object):

    def __init__(self,filename):
        self.filename = filename

        self.empty_line = re.compile("^\s*$")

        self.conn = sqlite3.connect(FileReader.database_name)
        self.cur = self.conn.cursor()
        #get the comment string from database
        match = re.search("\.[^.]*$",self.filename)
        if match:
            extension = match.group(0)
            print extension

        self.cur.execute("SELECT comment_regex FROM extensions WHERE extension = ?",(extension))
        self.cur.fetchmany()


        
    def count_lines(self):
        try:
            f = open(self.filename,"r")
        except (OSError,IOError):
            return None
        else:
            empty = 0
            for index,line in enumerate(f):
                #simple case if the line is not empty we count it
                if self.empty_line.match(line):
                    empty += 1
                if self.

            f.close()
            #index starts at zero so we add one
            total = index+1
            loc = total - empty - comments
            print loc,"/",total
        return ()
            





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

    #create a toy database
    database = "mtvr_db.sqlite"
    fr = FileReader()
    fr.count_lines()

    '''GUI PORTION
    root = tk.Tk()
    mw = MainWindow(root)
    mw.pack()

    root.mainloop()
    '''
