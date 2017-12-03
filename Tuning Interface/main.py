import serial

from Tkinter import *

import time

import threading

import tkMessageBox
import Tkinter
import re

import serial

serial_port = serial.Serial('com8', 115200) ## 2Mbps 2000000 !!


root = Tk()
root.title("ECU Tuning")
TurnOn = True
var = StringVar()
tableheight = 16
tablewidth = 16
serialdata = []

VETabData = []
IgnTabData = []
AFRTabData = []

temp = 0

rpmVar = StringVar()
mapVar = StringVar()
afrVar = StringVar()
batVar = StringVar()
tpsVar = StringVar()
colVar = StringVar()
iatVar = StringVar()
injVar = StringVar()
ignVar = StringVar()
veTVar = StringVar()
degTVar = StringVar()
afrTVar = StringVar()
gamVar = StringVar()


RPM = 0
MAP = 0
AFR = 0
BAT = 0
TPS = 0
COL = 0
IAT = 0
INJ = 0
IGN = 0
VET = 0
DEGT = 0
AFRT = 0
GAM = 0

def number_to_string(num, n=3):
    num_str = str(num)
    if len(num_str) < n:
        num_str = '0' * (n-len(num_str)) + num_str
    elif len(num_str) % n != 0:
        num_str = '0'*(n-len(num_str)%n) + num_str

    chars = re.findall('.'*n, num_str)
    l = [chr(int(i)) for i in chars]
    return ''.join(l)


def write_data(page, row, column, index, value):
    serial_port.write("P")
    serial_port.write(number_to_string(page, 3))
    serial_port.write("W")
    if index == 2:
        serial_port.write(number_to_string(column, 3))
        serial_port.write(number_to_string(1, 3))
        serial_port.write(number_to_string(value, 3))
    elif index == 3:
        serial_port.write(number_to_string(row + 16, 3))
        serial_port.write(number_to_string(1, 3))
        serial_port.write(number_to_string(value, 3))
    else:
        serial_port.write(number_to_string((row * 16 + column), 3))
        serial_port.write(number_to_string(0, 3))
        serial_port.write(number_to_string(value, 3))

def write_table():
    serial_port.write("P")
    serial_port.write(number_to_string(5, 3))
    serial_port.write("W")
    for i in range(256):
        serial_port.write(number_to_string(i, 3))
        serial_port.write(number_to_string(0, 3))
        serial_port.write(number_to_string(i, 3))
    serial_port.write("B")


def burn_data():
    serial_port.write("B")
    print("BUUUUUUUUURN!!!!!")

def init_table():
    serial_port.write("P")
    serial_port.write(number_to_string(1,3))
    serial_port.write("V")
    for i in range(16*16+16+16):
        value = serial_port.read().encode('hex')
        VETabData.append(value)

    serial_port.write("P")
    serial_port.write(number_to_string(3, 3))
    serial_port.write("V")
    for i in range(16 * 16 + 16 + 16):
        value = serial_port.read().encode('hex')
        IgnTabData.append(value)

    serial_port.write("P")
    serial_port.write(number_to_string(5, 3))
    serial_port.write("V")
    for i in range(16 * 16 + 16 + 16):
        value = serial_port.read().encode('hex')
        AFRTabData.append(value)



init_table()


def get_log(ser):
    try:
        while True:
            ser.write("A")
            for i in range(34):
                value = ser.read().encode('hex')
                serialdata.append(value)
            display_log()
            del serialdata[:]
            time.sleep(1)
        ser.close()
    except KeyboardInterrupt:
        exit()




thread = threading.Thread(target=get_log, args=(serial_port,))
thread.start()

def gantihal(frame):
    frame.tkraise()

def on_enter(page, arrayRow, arrayCol, index, variable):
    print "page:", page, "arrayRow:", arrayRow, "arrayCol:", arrayCol, "index:", index, "variable", variable.get()
    write_data(page, arrayRow, arrayCol, index, variable.get())





def display_log():
    global RPM, MAP, AFR, BAT, TPS, COL, IAT, INJ, IGN, VET, DEGT, AFRT, GAM
    RPM = int(serialdata[15] + serialdata[14], 16)
    MAP = int(serialdata[5] + serialdata[4], 16)
    BAT = int(serialdata[9], 16)
    AFR = int(serialdata[10], 16)
    TPS = int(serialdata[24], 16)
    COL = int(serialdata[7], 16)
    IAT = int(serialdata[6], 16)
    INJ = int(serialdata[21], 16)
    IGN = int(serialdata[3], 16)
    VET = int(serialdata[19], 16)
    DEGT = int(serialdata[23], 16)
    AFRT = int(serialdata[20], 16)
    GAM = int(serialdata[18], 16)

    rpmVar.set(str(RPM))
    mapVar.set(str(MAP))
    batVar.set(str(BAT))
    afrVar.set(str(AFR))
    tpsVar.set(str(TPS))
    colVar.set(str(COL))
    iatVar.set(str(IAT))
    injVar.set(str(INJ))
    ignVar.set(str(IGN))
    veTVar.set(str(VET))
    degTVar.set(str(DEGT))
    afrTVar.set(str(AFRT))
    gamVar.set(str(GAM))




kul = 1
naes = 2

f1 = Frame(root)
f2 = Frame(root)
f3 = Frame(root)
f4 = Frame(root)

for frame in (f1, f2, f3, f4):
    frame.grid(row=0, column=0, sticky='news')
    Button(frame, text='LOG', command=lambda: gantihal(f1)).grid(row=0, column=0)
    Button(frame, text='VE', command=lambda: gantihal(f2)).grid(row=0,column=1)
    Button(frame, text='IGN', command=lambda: gantihal(f3)).grid(row=0,column=2)
    Button(frame, text='AFR', command=lambda: gantihal(f4)).grid(row=0,column=3)

for frame in (f2, f3, f4):
    Button(frame, text='Burn', command=lambda: burn_data()).grid(row=0, column=5)


Label(f1,textvariable = rpmVar, bd=0, width=10, height=1).grid(row=1, column=6)
Label(f1,textvariable = mapVar, bd=0, width=10, height=1).grid(row=2, column=6)
Label(f1,textvariable = afrVar, bd=0, width=10, height=1).grid(row=3, column=6)
Label(f1,textvariable = batVar, bd=0, width=10, height=1).grid(row=4, column=6)
Label(f1,textvariable = tpsVar, bd=0, width=10, height=1).grid(row=5, column=6)
Label(f1,textvariable = colVar, bd=0, width=10, height=1).grid(row=6, column=6)
Label(f1,textvariable = iatVar, bd=0, width=10, height=1).grid(row=7, column=6)
Label(f1,textvariable = injVar, bd=0, width=10, height=1).grid(row=8, column=6)
Label(f1,textvariable = ignVar, bd=0, width=10, height=1).grid(row=9, column=6)


Label(f1,textvariable = veTVar, bd=0, width=10, height=1).grid(row=11, column=6)
Label(f1,textvariable = degTVar, bd=0, width=10, height=1).grid(row=12, column=6)
Label(f1,textvariable = afrTVar, bd=0, width=10, height=1).grid(row=13, column=6)
Label(f1,textvariable = gamVar, bd=0, width=10, height=1).grid(row=14, column=6)



Label(f1, text = "RPM:", justify = RIGHT).grid(row=1, column=5)
Label(f1, text = "MAP:", justify = RIGHT).grid(row=2, column=5)
Label(f1, text = "AFR:", justify = RIGHT).grid(row=3, column=5)
Label(f1, text = "BAT:", justify = RIGHT).grid(row=4, column=5)
Label(f1, text = "TPS:", justify = RIGHT).grid(row=5, column=5)
Label(f1, text = "COL:", justify = RIGHT).grid(row=6, column=5)
Label(f1, text = "IAT:", justify = RIGHT).grid(row=7, column=5)
Label(f1, text = "INJ:", justify = RIGHT).grid(row=8, column=5)
Label(f1, text = "IGN:", justify = RIGHT).grid(row=9, column=5)

Label(f1, text = "VE T:", justify = RIGHT).grid(row=11, column=5)
Label(f1, text = "DEG T:", justify = RIGHT).grid(row=12, column=5)
Label(f1, text = "AFR T:", justify = RIGHT).grid(row=13, column=5)
Label(f1, text = "GAMMA:", justify = RIGHT).grid(row=14, column=5)


Label(f2, text="VE", bg="yellow").grid(row=19, column=5)
Label(f2, text="RPM", bg="yellow").grid(row=20, column=5)
Label(f2, text="MAP", bg="yellow").grid(row=21, column=5)
Label(f2,textvariable = veTVar, bd=0, width=5, height=1).grid(row=19, column=6)
Label(f2,textvariable = rpmVar, bd=0, width=5, height=1).grid(row=20, column=6)
Label(f2,textvariable = mapVar, bd=0, width=5, height=1).grid(row=21, column=6)
counter = 0
for row in xrange(tableheight):
    for column in xrange(tablewidth):
        strVar1 = StringVar()
        page = 1
        index = 1
        entry = Entry(f2, textvariable=strVar1, width=5)
        entry.insert(END, int(VETabData[counter], 16))
        entry.grid(row=15 - row + 2, column=column + 6)
        entry.focus()
        entry.bind('<Return>', lambda event, strVar = strVar1, page = page, row = row, col = column, val = index: on_enter(page, row, col, val, strVar))
        counter = counter + 1


Label(f2, text="RPM", bg="gray").grid(row=18, column=22)
counter = 0
for column in xrange(tablewidth):
    strVar3 = StringVar()
    page = 1
    index = 2
    entry = Entry(f2, textvariable=strVar3, width=5, bg="gray")
    entry.insert(END, int(VETabData[256 + counter], 16))
    entry.grid(row=18, column=column + 6)
    entry.focus()
    entry.bind('<Return>', lambda event, strVar = strVar3, page = page, row = 0, col = column, val = index: on_enter(page, row, col, val, strVar))
    counter = counter + 1


Label(f2, text="LOAD", bg="gray").grid(row=1, column=5)
counter = 0
for row in xrange(tableheight):
    strVar2 = StringVar()
    page = 1
    index = 3
    entry = Entry(f2, textvariable=strVar2, width=5, bg="gray")
    entry.insert(END, int(VETabData[272 + counter], 16))
    entry.grid(row=15 - row + 2, column=5)
    entry.focus()
    entry.bind('<Return>', lambda event, strVar = strVar2, page = page, row = row, col = 5, val = index: on_enter(page, row, col, val, strVar))
    counter = counter + 1







Label(f3, text="DEG", bg="yellow").grid(row=19, column=5)
Label(f3, text="RPM", bg="yellow").grid(row=20, column=5)
Label(f3, text="MAP", bg="yellow").grid(row=21, column=5)
Label(f3,textvariable = degTVar, bd=0, width=5, height=1).grid(row=19, column=6)
Label(f3,textvariable = rpmVar, bd=0, width=5, height=1).grid(row=20, column=6)
Label(f3,textvariable = mapVar, bd=0, width=5, height=1).grid(row=21, column=6)
counter = 0
for row in xrange(tableheight):
    for column in xrange(tablewidth):
        strVar1 = StringVar()
        page = 3
        index = 1
        entry = Entry(f3, textvariable=strVar1, width=5)
        entry.insert(END, int(IgnTabData[counter], 16))
        entry.grid(row=15 - row + 2, column=column + 6)
        entry.focus()
        entry.bind('<Return>', lambda event, strVar = strVar1, page = page, row = row, col = column, val = index: on_enter(page, row, col, val, strVar))
        counter = counter + 1


Label(f3, text="RPM", bg="gray").grid(row=18, column=22)
counter = 0
for column in xrange(tablewidth):
    strVar3 = StringVar()
    page = 3
    index = 2
    entry = Entry(f3, textvariable=strVar3, width=5, bg="gray")
    entry.insert(END, int(IgnTabData[256 + counter], 16))
    entry.grid(row=18, column=column + 6)
    entry.focus()
    entry.bind('<Return>', lambda event, strVar = strVar3, page = page, row = 0, col = column, val = index: on_enter(page, row, col, val, strVar))
    counter = counter + 1


Label(f3, text="LOAD", bg="gray").grid(row=1, column=5)
counter = 0
for row in xrange(tableheight):
    strVar2 = StringVar()
    page = 3
    index = 3
    entry = Entry(f3, textvariable=strVar2, width=5, bg="gray")
    entry.insert(END, int(IgnTabData[272 + counter], 16))
    entry.grid(row=15 - row + 2, column=5)
    entry.focus()
    entry.bind('<Return>', lambda event, strVar = strVar2, page = page, row = row, col = 5, val = index: on_enter(page, row, col, val, strVar))
    counter = counter + 1










Label(f4, text="AFR", bg="yellow").grid(row=19, column=5)
Label(f4, text="RPM", bg="yellow").grid(row=20, column=5)
Label(f4, text="MAP", bg="yellow").grid(row=21, column=5)
Label(f4,textvariable = afrTVar, bd=0, width=5, height=1).grid(row=19, column=6)
Label(f4,textvariable = rpmVar, bd=0, width=5, height=1).grid(row=20, column=6)
Label(f4,textvariable = mapVar, bd=0, width=5, height=1).grid(row=21, column=6)
counter = 0
for row in xrange(tableheight):
    for column in xrange(tablewidth):
        strVar1 = StringVar()
        page = 5
        index = 1
        entry = Entry(f4, textvariable=strVar1, width=5)
        entry.insert(END, int(AFRTabData[counter], 16))
        AFRTabData
        entry.grid(row=15 - row + 2, column=column + 6)
        entry.focus()
        entry.bind('<Return>', lambda event, strVar = strVar1, page = page, row = row, col = column, val = index: on_enter(page, row, col, val, strVar))
        counter = counter + 1


Label(f4, text="RPM", bg="gray").grid(row=18, column=22)
counter = 0
for column in xrange(tablewidth):
    strVar3 = StringVar()
    page = 5
    index = 2
    entry = Entry(f4, textvariable=strVar3, width=5, bg="gray")
    entry.insert(END, int(AFRTabData[256 + counter], 16))
    entry.grid(row=18, column=column + 6)
    entry.focus()
    entry.bind('<Return>', lambda event, strVar = strVar3, page = page, row = 0, col = column, val = index: on_enter(page, row, col, val, strVar))
    counter = counter + 1


Label(f4, text="LOAD", bg="gray").grid(row=1, column=5)
counter = 0
for row in xrange(tableheight):
    strVar2 = StringVar()
    page = 5
    index = 3
    entry = Entry(f4, textvariable=strVar2, width=5, bg="gray")
    entry.insert(END, int(AFRTabData[272 + counter], 16))
    entry.grid(row=15 - row + 2, column=5)
    entry.focus()
    entry.bind('<Return>', lambda event, strVar = strVar2, page = page, row = row, col = 5, val = index: on_enter(page, row, col, val, strVar))
    counter = counter + 1




#entry.grid(row=1, column=0)

#L1 = Label(f2, text="User Name").grid(row=21,column=0)
#E1 = Entry(f2)
#E1.insert(END, kul)
#E1.grid(row=22,column=0)



gantihal(f1)

root.mainloop()