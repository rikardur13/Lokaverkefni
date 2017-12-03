import Tkinter as tk

def rand_func(arrayRow, arrayCol, inputVal):
    print "arrayRow:", arrayRow, "arrayCol:", arrayCol, "inputVal:", inputVal

root = tk.Tk()
entry = tk.Entry()
entry.pack(padx=5, pady=5)
entry.focus()
myRowNumber = 10
myColNumber = 20
myCounterNumber = 30
entry.bind('<Return>',lambda event, row=myRowNumber, col=myColNumber, val=myCounterNumber: rand_func(row, col, val))
root.mainloop()