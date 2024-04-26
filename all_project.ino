#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
// Include Libraries
#include "Arduino.h"
#include "RTClib.h"
#include <Servo.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int buzzer = 9; //buzzer to arduino pin 9
SoftwareSerial gsmSerial(10, 5); //RX, TX
// Declare the Servo pin 
int servoPin = 3; 
// Create a servo object 
Servo Servo1;

RTC_DS3231 rtcDS;
// Pin Definitions
#define SIM800L_SOFTWARESERIAL_PIN_TX  5
#define SIM800L_SOFTWARESERIAL_PIN_RX 10

const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

void setup() {
   
   Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
   
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

        tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
    

  // put your setup code here, to run once:
   lcd.begin();
  lcd.backlight();
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Smart Pill Box");
  
    
 gsmSerial.begin(9600); // Setting the baud rate of GSM Module
   delay(1000);
   Serial.println("Preparing to send SMS");
   SendMessage();
    // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin); 
if (! rtcDS.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
    }
    if (rtcDS.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtcDS.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtcDS.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    
}

void loop() {
   // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   delay(1000); 
   // Make servo go to 180 degrees 
   Servo1.write(180); 
   delay(1000); 
  // put your main code here, to run repeatedly:
  
    if(menuOption == '1') {
    // RTC - Real Time Clock - Test Code
    //This will display the time and date of the RTC. see RTC.h for more functions such as rtcDS.hour(), rtcDS.month() etc.
    DateTime now = rtcDS.now();
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print("  ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(1000);
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    }



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\n Smart Pill Box?"));
    Serial.println(F("(1) Send SMS"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
          Serial.println(F("Now Testing RTC - Real Time Clock"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

void SendMessage()
{
   Serial.println("Setting the GSM in text mode");
   gsmSerial.println("AT+CMGF=1\r");
   delay(2000);
   Serial.println("Sending SMS to the desired phone number!");
   gsmSerial.println("AT+CMGS=\"+91\"\r");
   // Replace x with mobile number
   delay(2000);
 
   Serial.println("Reminder for your tablet");
   Serial.println("SMS Sent");
    gsmSerial.println("Reminder for your tablet");    // SMS Text
   delay(200);
   gsmSerial.println((char)26);               // ASCII code of CTRL+Z
   delay(2000);
}
