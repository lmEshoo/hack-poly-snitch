#include <LiquidCrystal.h>
#include <Servo.h>

//LCD Hookups
#define RS 10
#define EN 11
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define LCD_BACKLIGHT A4



LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

void setup(){
   Serial1.begin(9600);
   Serial.begin(9600);

   lcdSetup();
   
   delay(500);
}
void loop(){


}//loop

void lcdSetup(){
  pinMode(LCD_BACKLIGHT,OUTPUT);
  digitalWrite(LCD_BACKLIGHT,HIGH);
  lcd.begin(16, 2);
  lcd.print("    THE ");
  lcd.setCursor(1,1);
  lcd.print("        SNITCH");
}  


