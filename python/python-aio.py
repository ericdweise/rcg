
import subprocess
import mysql.connector
from mysql.connector import Error
from mysql.connector import errorcode
import serial
import sys
import io
import time
import datetime

d1 = .0015
d2 = .25

class sensor(object):
  def __init__(self, uniName, command, address, ardName):
    self.uniName = uniName
    self.command = command
    self.address = address
    self.ardName = ardName

  def getData(self):
    data = send(self.command, self.address)
    print self.uniName
    print data
    time.sleep(d1)
    timestamp = getTime()
    print timestamp
    return data
  
  def printObject(self):
    print self.uniName
    print self.command
    print self.address
    print self.ardName

  def establishSerial(arduinoNum):
    ardSerial = serial.Serial(arduinoNum, 9600)
    return ardSerial

  def stringArray(array):
    string = ''
    for x in array:
      string += x
    return string

  def send(commandCode, arduinoNum):
    ardDataBuffer = []
    endOfData = 0
    endOfDataChar = '$'
    errorChar = '@'
    dataReturn = ''

    ardSerial = establishSerial(arduinoNum)

    #write the actual command code the arduino is to execute
    while endOfData == 0:
      myData = []
      while ardSerial.in_waiting == 0:
        time.sleep(1)
        ardSerial.write('<' + commandCode + '>')
        print "command code sent"
        time.sleep(2)
      myData = ardSerial.read()
      ardDataBuffer += myData
      if endOfDataChar in myData:
        dataReturn = stringArray(ardDataBuffer)
        endOfData = 1

      if errorChar in myData:
        dataReturn = 'error: ' + stringArray(ardDataBuffer)
        endOfData = 1

    #remove the $ character from the results
    finalData = dataReturn[:-1]

    #close the serial port
    ardSerial.close()
    try:
      sys.stdout.flush()
    except:
      pass

    #return the data to the python-master program
    return finalData

  #function to get current time
  def getTime():
    ts = time.time()
    timestamp = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
    return timestamp

  #function to insert data to local database
  def localDB(self, datetime, reading):
    createTableLocal(self.uniName)
    try:
      connection = mysql.connector.connect(host='localhost', database='sensors', user='root', password='8693ViaMallorca')
      sql_insert_query = "INSERT INTO " + self.uniName + " (datetime, reading) VALUES('" + datetime + "', '" + reading + "');"
      print sql_insert_query
      cursor = connection.cursor()
      result = cursor.execute(sql_insert_query)
      connection.commit()
      print "db record sent"
    except:
      print "failed db record insert"

def stringArray(array):
  string = ''
  for x in array:
    string += x
  return string

def establishSerial(arduinoNum):
  ardSerial = serial.Serial(arduinoNum, 9600)
  return ardSerial

#function to create table if it does not exist
def createTableLocal(table):
  try:
    connection = mysql.connector.connect(host='localhost', database='sensors', user='root', password='8693ViaMallorca')
    sql_insert_query = "CREATE TABLE IF NOT EXISTS " + table + " (id INT AUTO_INCREMENT NOT NULL, reading VARCHAR(8) NOT NULL, datetime DATETIME NOT NULL, PRIMARY KEY (id)) ENGINE=INNODB;"
    print sql_insert_query
    cursor = connection.cursor()
    result = cursor.execute(sql_insert_query)
    connection.commit()
    print "table creation or was already successful"
  except:
    print "failed table creation"

def send(commandCode, arduinoNum):
  ardDataBuffer = []
  endOfData = 0
  endOfDataChar = '$'
  errorChar = '@'
  dataReturn = ''

  ardSerial = establishSerial(arduinoNum)

  #write the actual command code the arduino is to execute
  while endOfData == 0:
    myData = []
    while ardSerial.in_waiting == 0:
      time.sleep(1)
      ardSerial.write('<' + commandCode + '>')
      print "command code sent"
      time.sleep(2)
    myData = ardSerial.read()
    ardDataBuffer += myData
    if endOfDataChar in myData:
      dataReturn = stringArray(ardDataBuffer)
      endOfData = 1

    if errorChar in myData:
      dataReturn = 'error: ' + stringArray(ardDataBuffer)
      endOfData = 1

  #remove the $ character from the results
  finalData = dataReturn[:-1]

  #close the serial port
  ardSerial.close()
  try:
    sys.stdout.flush()
  except:
    pass
  
  #return the data to the python-master program
  return finalData

#return the list of ports occupied by arduinos on the rpi system
def checkArduinos():
  data = subprocess.check_output("ls /dev/ttyACM*", shell=True)
  return data

#check the system for arduinos and have them all reports identities  
def getAddresses():
  addresses = checkArduinos().splitlines()
  arduinosRaw = ['0'] * len(addresses)
  for x in range(len(addresses)):
    identity = send("9999", addresses[x])
    arduinosRaw[x] = [addresses[x], identity]
  return arduinosRaw    

#function to get current time
def getTime():
  ts = time.time()
  timestamp = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S') 
  return timestamp

def parseSensors(arduinos):
  sensors = {}
  for arduino in arduinos:
    address = arduino[0]
    #print "address is: " + address
    raw1 = arduino[1].split(":")
    ardName = raw1[0]
    #print "ardName is: " + ardName
    raw2 = raw1[1].split(",")
    #print raw2
    for double in raw2:
      pair = double.split(";")
      command = pair[0]
      #print "command is: " + command
      engName = pair[1]
      #print "engName is: " + engName
      sensorObj = sensor(engName, command, address, ardName)
      sensorList = ""
      for x in sensors:
        sensorList = sensorList + x
      print sensorList
      chanel = "a"
      if engName in sensorList:
        chanel = chanel + "a"
        engName + chanel
      sensors[engName] = sensorObj
  return sensors
#setup of the program getting ready for main loop
sensors = {}
arduinos = getAddresses()
sensors = parseSensors(arduinos)

print "main print statement"
for x in arduinos:
  print x

for y in sensors:
  print (y)

#main loop to monitor conditions and adjust system
while 1:
  temp1 = sensors["temp1"].getData()
  timestamp = getTime()
  sensors["temp1"].localDB(timestamp, temp1)
  time.sleep(1);
