import serial, time, visa
import os,sys, excel
# (port='COM1', baudrate=19200, bytesize=8, parity='N',
# stopbits=1, timeout=None, xonxoff=0, rtscts=0)

def open_serial_port(port):
    # initialization and open the port
    #  possible timeout values:
    #    1. None: wait forever, block call
    #    2. 0: non-blocking mode, return immediately
    #    3. x, x is bigger than 0, float allowed, timeout block call
    ser = serial.Serial()
    ser.port = port #this is ser.name
    ser.baudrate = 115200
    ser.bytesize = serial.EIGHTBITS  # number of bits per bytes
    ser.parity = serial.PARITY_NONE  # set parity check: no parity
    ser.stopbits = serial.STOPBITS_ONE  # number of stop bits
    # ser.timeout = None          #block read
    ser.timeout = 1  # non-block read
    # ser.timeout = 2           #timeout block read
    ser.xonxoff = True  # disable software flow control
    ser.rtscts = False  # disable hardware (RTS/CTS) flow control
    ser.dsrdtr = False  # disable hardware (DSR/DTR) flow control
    ser.writeTimeout = 2  # timeout for write
    try:
        ser.open()
        ser.flushInput()  # flush input buffer, discarding all its contents
        ser.flushOutput()  # flush output buffer, aborting current output
    except Exception:
        print('Error open serial port '+ser.name)
        # exit()
    if ser.is_open:
        try:
            ser.flushInput()    # flush input buffer, discarding all its contents
            ser.flushOutput()   # flush output buffer, aborting current output
                                # and discard all that is in buffer
        except Exception:
            print('Error port cleaning ' + ser.name)
            exit()

    return ser
def serial_portRW(ser):
    try:
        # write data
        # The program send out AT commands to query modem signal strength.
        # If you want to know more about modem AT commands, refer to post here.
        #ser.write(b'cmdstop')
        #ser.write('wifitxout 1 0x0 100\n\r'.encode())
        #ser.write(('wifitxout 1 0x0 100'))
        print("write data: successful!")

        time.sleep(1)  # give the serial port sometime to receive the data

        numOfLines = 0

        while True:
            response = ser.readline()
            print("read data: " + response)
            numOfLines = numOfLines + 1
            if (numOfLines >= 1):
                break
    except Exception:
        print(ser.name + ' error RW...')
rm = visa.ResourceManager()
print(rm.list_resources())

ser=open_serial_port('COM6')
print(ser.name)
# serial_portRW(ser)

ser.write('wifitxout 1 0x0 100\n\r'.encode())
response = ser.readline()
print(response)
ser.write('cmdstop\n\r'.encode())
response = ser.readline()
print(response)
ser.write('cmdstop\n\r'.encode())
response = ser.readline()
print(response)
len=0
while True:
    response = ser.readline()
    print(response)
    len=len+1
    if len>5:
        break
response = ser.readline()
print(response)
#print(os.system('MODE.COM'))

#cmd = 'Enter command or exit:\n\r'.encode()
# cmd.encode('ascii')
# ser.write(cmd)

ser.close()

