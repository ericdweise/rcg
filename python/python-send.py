import serial
import sys
import time, threading

arduinoNum = "0"

ardSerial = serial.Serial('/dev/ttyACM0', 9600)

def foo():
	ardSerial.write("1111>")
	time.sleep(1)
	ardSerial.write("2222>")
	time.sleep(1)
	ardSerial.write("3333>")
	time.sleep(3)
	ardSerial.write("4444>")
	time.sleep(3)
	ardSerial.write("5555>")
	time.sleep(3)
	threading.Timer(3, foo).start()

foo()

