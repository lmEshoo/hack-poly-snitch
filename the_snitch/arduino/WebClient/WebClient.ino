/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */
 
 
#include <LiquidCrystal.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include "pitches.h"

#define PORTX 3000

//LCD Hookups
#define RS 10
#define EN 11
#define D4 2
#define D5 3
#define D6 4
#define D7 5
#define LCD_BACKLIGHT A5

// LCD Special Characters
byte like[8] = {
  B00000,
  B00100,
  B00100,
  B11011,
  B11001,
  B11001,
  B11111,
};
byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(134,71,249,154); // Google
//IPAddress server(192,30,252,153); // Google
 

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };
  
//LCD Setup
LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
#define W5200_CS  10
#define SDCARD_CS 4
void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   lcd.begin(16, 2);
   pinMode(A5, OUTPUT);
   digitalWrite(A5, HIGH);
  //Setup LCD Special Characters
  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);
  lcd.createChar(5, like);
  lcd.setCursor(0,0);
  lcd.print(" THE SNITCH ");
  lcd.write(5);
  progBar();
  lcd.clear();
  pinMode(SDCARD_CS,OUTPUT);
 digitalWrite(SDCARD_CS,HIGH);//Deselect the SD card
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //lcd.clear();
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  /*lcd.print("                         ");
  lcd.setCursor(0,0);
  lcd.print("connecting...");
  lcd.clear();*/
  // if you get a connection, report back via serial:
  if (client.connect(server, PORTX)) {
//lcd.clear();
    Serial.println("connected");
    //lcd.print("Searching");
    
    // Make a HTTP request:
    client.println("GET /");
    //client.println("GET /the-snitch HTTP/1.1");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  //lcd.clear();
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  //for(int i=0;i<3;i++){
    
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        if(c=='$'){ Serial.println("BLAHHHH"); 
        //
            for (int thisNote = 0; thisNote < 8; thisNote++) {
        
            // to calculate the note duration, take one second 
            // divided by the note type.
            //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
            int noteDuration = 1000/noteDurations[thisNote];
            tone(8, melody[thisNote],noteDuration);
        
            // to distinguish the notes, set a minimum time between them.
            // the note's duration + 30% seems to work well:
            int pauseBetweenNotes = noteDuration * 1.30;
            delay(pauseBetweenNotes);
            // stop the tone playing:
            noTone(8);
          }//for
          delay(50);
        }
      }
    delay(10);
      // if the server's disconnected, stop the client:
      if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        
          // give the Ethernet shield a second to initialize:
          delay(1000);
          Serial.println("connecting...");
        
          // if you get a connection, report back via serial:
          if (client.connect(server, PORTX)) {
            Serial.println("connected");
            // Make a HTTP request:
            client.println("GET /");
            client.println();
          } 
          else {
            // kf you didn't get a connection to the server:
            Serial.println("connection failed");
          }
        // do nothing forevermore:
        //for(;;)
         // ;
     }
  //}//for
} //loop

void progBar(){
 lcd.setCursor(0,1);
 lcd.print("     ");
 for (int i = 0; i<16; i++){
   for (int j=0; j<5; j++){
      lcd.setCursor(i, 1);   
      lcd.write(j);
      delay(50);
   }
 }
 delay(500);
 lcd.clear();
 delay(1500);
}  //progBar
