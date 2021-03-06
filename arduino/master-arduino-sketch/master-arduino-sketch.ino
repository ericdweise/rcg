// include the library LCD code:
#include <LiquidCrystal.h>

//for ds18b20 code
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_PIN 2

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

  //device addresses to be found in another sketch and assigned here

DeviceAddress Probe01 = { 0x28, 0xAA, 0x06, 0xC3, 0x17, 0x13, 0x02, 0x95 }; 
DeviceAddress Probe02 = { 0x28, 0x27, 0x2B, 0xAB, 0x1F, 0x13, 0x01, 0x6A };
DeviceAddress Probe03 = { 0x28, 0xFF, 0x7B, 0xCC, 0xC1, 0x16, 0x04, 0xA1 };
DeviceAddress Probe04 = { 0x28, 0xFF, 0x4F, 0x19, 0xC2, 0x16, 0x04, 0x5E };
//ds18b20 code end

//for relay control code
// declare and name all actuator pins on the arduino


//relay control code end
//LCD Setup Code
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//declare command code variable
String commandCode = "0";

//tentacle sheild code 
#include <SoftwareSerial.h>         //Include the software serial library  

SoftwareSerial sSerial(11, 10);     // RX, TX  - Name the software serial library sftSerial (this cannot be omitted)
                                    // assigned to pins 10 and 11 for maximum compatibility
int s0 = 7;                         // Tentacle uses pin 7 for multiplexer control S0
int s1 = 6;                         // Tentacle uses pin 6 for multiplexer control S1
int enable_1 = 5;                   // Tentacle uses pin 5 to control pin E on shield 1
int enable_2 = 4;                   // Tentacle uses pin 4 to control pin E on shield 2

char computerdata[20];              // A 20 byte character array to hold incoming data from a pc/mac/other
char sensordata[30];                // A 30 byte character array to hold incoming data from the sensors
byte computer_bytes_received = 0;   // We need to know how many characters bytes have been received
byte sensor_bytes_received = 0;     // We need to know how many characters bytes have been received

char *channel;                      // Char pointer used in string parsing
char *cmd;                          // Char pointer used in string parsing

String str;
String str_channel;
//tentacle sheild code end

//Serial buffer code
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
//Serial buffer code end



void setup() {
  
  Serial.begin(9600);
  //LCD Code
  lcd.begin(16, 2);
  
  //for ds18b20 code
  sensors.begin();
  sensors.setResolution(Probe01, 12);
  sensors.setResolution(Probe02, 12);
  sensors.setResolution(Probe03, 12);
  sensors.setResolution(Probe04, 12);
  //ds18b20 code end

  //tentacle sheild code
  pinMode(s0, OUTPUT);             // set the digital output pins for the serial multiplexer
  pinMode(s1, OUTPUT);
  pinMode(enable_1, OUTPUT);
  pinMode(enable_2, OUTPUT);

  sSerial.begin(9600);             // Set the soft serial port to 9600 (change if all your devices use another baudrate)
  intro();                         // display startup message
  //tentacle sheild code end

  //relay control code


  //relay control code end
  
}



void loop() {
  
  //ds18b20 generic code
  sensors.requestTemperatures();
  delay(100);
  //end ds18b20 generic code

  // serial buffer code
  

  // read serial data if its available and set command code
//  if(Serial.available()){
//    commandCode = Serial.read() - '0';
//    delay(50);
//  } 

  // Serial buffer code
  recvWithStartEndMarkers();
  showNewData();
  //Main set of if statements to trigger conditions from Serial commands
      
  if(commandCode == "1111"){
    Serial.print("Temperature Probe 1: ");
    float temp;
    temp = printTemperature(Probe01);
    lcd.setCursor(0,0);
    lcd.print("Temp1");
    lcd.setCursor(1,0);
    lcd.print(temp);
    commandCode = "0";
  }
  
  if(commandCode == "2222"){
    Serial.print("Temperature Probe 2: ");
    printTemperature(Probe02);
      commandCode = "0";
  }
  
  if(commandCode == "3333"){
    str_channel = String(channel);
    computer_bytes_received = 3;                  // Character length is always 3 for read commands
    str = str_channel.concat(":r");                  // Create command 
    str.toCharArray(computerdata, 20);             // Convert from string to char, the desired data type
    computerdata[0] = 48;
    computerdata[1] = 58;
    computerdata[2] = 114;
 
    if (computer_bytes_received != 0) {             // If computer_bytes_received does not equal zero
        channel = strtok(computerdata, ":");          // Let's parse the string at each colon
      cmd = "r";
        open_channel();                               // Call the function "open_channel" to open the correct data path      
        if (cmd != 0) {                               // if no command has been sent, send nothing
          sSerial.print(cmd);                         // Send the command from the computer to the Atlas Scientific device using the softserial port
          sSerial.print("\r");                        // <CR> carriage return to terminate message
        }
      computer_bytes_received = 0;                  // Reset the var computer_bytes_received
    }
    for(int i = 0; i<2; i++){
      if (sSerial.available() > 0) {                 // If data has been transmitted from an Atlas Scientific device
        sensor_bytes_received = sSerial.readBytesUntil(13, sensordata, 30); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received
        sensordata[sensor_bytes_received] = 0;       // we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
        Serial.print("pH Data: ");
        Serial.println(sensordata);
      }
    }
    commandCode = "0";
  }
  
  if(commandCode == "4444"){
    str_channel = String(channel);
    computer_bytes_received = 3;                  // Character length is always 3 for read commands
    str = str_channel.concat(":r");                  // Create command 
    str.toCharArray(computerdata, 20);             // Convert from string to char, the desired data type
    computerdata[0] = 49;
    computerdata[1] = 58;
    computerdata[2] = 114;
 
    if (computer_bytes_received != 0) {             // If computer_bytes_received does not equal zero
        channel = strtok(computerdata, ":");          // Let's parse the string at each colon
      cmd = "r";
        open_channel();                               // Call the function "open_channel" to open the correct data path      
        if (cmd != 0) {                               // if no command has been sent, send nothing
          sSerial.print(cmd);                         // Send the command from the computer to the Atlas Scientific device using the softserial port
          sSerial.print("\r");                        // <CR> carriage return to terminate message
        }
      computer_bytes_received = 0;                  // Reset the var computer_bytes_received
      }
  
    for(int i = 0; i<2; i++){
      if (sSerial.available() > 0) {                 // If data has been transmitted from an Atlas Scientific device
        sensor_bytes_received = sSerial.readBytesUntil(13, sensordata, 30); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received
        sensordata[sensor_bytes_received] = 0;       // we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
        Serial.print("DO Data: ");
        Serial.println(sensordata);                  // let’s transmit the data received from the Atlas Scientific device to the serial monitor
      }
    }
    commandCode = "0";
  }

  if(commandCode == "5555"){
    str_channel = String(channel);
    computer_bytes_received = 3;                  // Character length is always 3 for read commands
    str = str_channel.concat(":r");                  // Create command 
    str.toCharArray(computerdata, 20);             // Convert from string to char, the desired data type
    computerdata[0] = 50;
    computerdata[1] = 58;
    computerdata[2] = 114;
 
    if (computer_bytes_received != 0) {             // If computer_bytes_received does not equal zero
        channel = strtok(computerdata, ":");          // Let's parse the string at each colon
      cmd = "r";
        open_channel();                               // Call the function "open_channel" to open the correct data path      
        if (cmd != 0) {                               // if no command has been sent, send nothing
          sSerial.print(cmd);                         // Send the command from the computer to the Atlas Scientific device using the softserial port
          sSerial.print("\r");                        // <CR> carriage return to terminate message
        }
      computer_bytes_received = 0;                  // Reset the var computer_bytes_received
      }

    for(int i = 0; i<2; i++){
      if (sSerial.available() > 0) {                 // If data has been transmitted from an Atlas Scientific device
        sensor_bytes_received = sSerial.readBytesUntil(13, sensordata, 30); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received
        sensordata[sensor_bytes_received] = 0;       // we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
        Serial.print("EC Data: ");
        Serial.println(sensordata);                  // let’s transmit the data received from the Atlas Scientific device to the serial monitor
      }
      delay(20);
    }
    commandCode = "0";
  }

  if(commandCode == "9999"){
    String identity = "Arduino4";
    Serial.print(identity);
    commandCode = "0";

  }

  
//End of Main set of if statements 
        
}//end of main loop

//start of functions needed to be called in them main loop

//Serial buffer functions
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
 // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
                
                commandCode = receivedChars;
                String String(commandCode);
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
 if (newData == true) {
 Serial.println(receivedChars);
 newData = false;
 }
}

//ds18b20 code functions
float printTemperature(DeviceAddress deviceAddress)
{

float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print("C: ");
   Serial.print(tempC);
   Serial.print(" F: ");
   Serial.println(DallasTemperature::toFahrenheit(tempC));
   return(DallasTemperature::toFahrenheit(tempC));
   }
}
// ds18b20 end code

//tentacle sheild code
// Print intro
void intro() {
  Serial.flush();
  Serial.println(" ");
}

// Open a channel via the Tentacle serial multiplexer
void open_channel() {

  switch (*channel) {

    case '0':                                // if channel==0 then we open channel 0
      digitalWrite(enable_1, LOW);           // setting enable_1 to low activates primary channels: 0,1,2,3
      digitalWrite(enable_2, HIGH);          // setting enable_2 to high deactivates secondary channels: 4,5,6,7
      digitalWrite(s0, LOW);                 // S0 and S1 control what channel opens
      digitalWrite(s1, LOW);                 // S0 and S1 control what channel opens
      break;

    case '1':
      digitalWrite(enable_1, LOW);
      digitalWrite(enable_2, HIGH);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      break;

    case '2':
      digitalWrite(enable_1, LOW);
      digitalWrite(enable_2, HIGH);
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      break;

    case '3':
      digitalWrite(enable_1, LOW);
      digitalWrite(enable_2, HIGH);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      break;

    case '4':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      break;

    case '5':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      break;

    case '6':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      break;

    case '7':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      break;
  }
}
//tentacle sheild code end

