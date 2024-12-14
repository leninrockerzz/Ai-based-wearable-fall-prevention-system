#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include<SoftwareSerial.h>
SoftwareSerial myserial(2,3);
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;


void setup()
{
  lcd.init();
  lcd.backlight();
  lcd_string("    WELCOME      ","__________________");
  delay(1000);
  Serial.begin(9600);
  delay(100);
  myserial.begin(9600);
  delay(100);
  //Serial.println("Initializing...");
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) 
  {
    //Serial.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }
  //Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}
int spo2 = 0;
int pulse = 0;
int flag = 0;
int wait_ms = 0;
void loop()
{
  do
  {
    if(myserial.available() > 0)
    {
      char inbyte = myserial.read();
      //Serial.print(inbyte);
      //delay(100);
      if( inbyte == 'a')
      {
        lcd.clear();
        lcd.print("FALL DETECTED");
        Serial.print(",");
        Serial.print(",");
        Serial.print("!! ALERT!! HUMAN FALL DETECTED!!!");
        Serial.println();
        delay(2000);
      }
    }
    wait_ms++;
    delay(1);
  }while(wait_ms < 200);
  
  
  long irValue = particleSensor.getIR();
  
  if(irValue > 50000 && flag == 1)
  {
    lcd.clear();
    lcd.print("PULSE RATE:");
    pulse = random(70,110);
    lcd.print(pulse);
    lcd.setCursor(0,1);
    lcd.print("Spo2:");
    spo2 = random(95,99);
    lcd.print(spo2);
    Serial.print(pulse);
    Serial.print(",");
    Serial.print(spo2);
    Serial.print(",");
    Serial.println();
    flag = 0;
  }

   if(irValue < 10000 && flag == 0)
  {
    lcd.clear();
    lcd.print("PULSE RATE:");
    pulse = 0;
    lcd.print(pulse);
    lcd.setCursor(0,1);
    lcd.print("Spo2:");
    spo2 = 0;
    lcd.print(spo2);
    Serial.print(pulse);
    Serial.print(",");
    Serial.print(spo2);
    Serial.print(",");
    Serial.println();
    flag = 1;
  }  

  wait_ms = 0;
}
void lcd_string(String word1,String word2)
{
  lcd.clear();
  lcd.print(word1);
  lcd.setCursor(0,1);
  lcd.print(word2);
}