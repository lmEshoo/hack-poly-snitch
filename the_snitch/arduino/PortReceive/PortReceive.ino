
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>

//LCD Hookups
#define RS 10
#define EN 11
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define LCD_BACKLIGHT A4

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

// software serial #1: TX = digital pin 10, RX = digital pin 11
SoftwareSerial portOne(2,3);
LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);
// software serial #2: TX = digital pin 8, RX = digital pin 9
// on the Mega, use other pins instead, since 8 and 9 don't work on the Mega
//SoftwareSerial portTwo(8,9);

void setup()
{
  lcdSetup();
   
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
//   while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }

  // Start each software serial port
  portOne.begin(9600);
}

void loop()
{
  // By default, the last intialized port is listening.
  // when you want to listen on a port, explicitly select it:
//  portOne.listen();
//  Serial.println("Data from port one:");
//  // while there is data coming in, read it
//  // and send to the hardware serial port:
//  while (portOne.available() > 0) {
//    char inByte = portOne.read();
//    Serial.write(inByte);
//    lcd.setCursor(1,1);
//    lcd.write(inByte);
//  }
//  
  // blank line to separate data from the two ports:
  Serial.println();

}

void lcdSetup(){
  lcd.createChar(5, like);
   lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);
  pinMode(LCD_BACKLIGHT,OUTPUT);
  digitalWrite(LCD_BACKLIGHT,HIGH);
  lcd.begin(16, 2);
  lcd.print("  THE SNITCH ");
  lcd.write(5);
  progBar(); 
 // lcd.write(5);
}  
void progBar(){
 lcd.setCursor(0,1);
 lcd.print("                ");
 for (int i = 0; i<16; i++){
   for (int j=0; j<5; j++){
      lcd.setCursor(i, 1);   
      lcd.write(j);
      delay(50);
   }
 }
}  //progBar



