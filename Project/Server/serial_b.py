import serial
import sys
import time
ser = serial.Serial('/dev/ttyACM0',9600)
ser.write('s') 
time.sleep(2.0)
ser.write('e') 
sys.exit()
