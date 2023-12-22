#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h> //Reference infrared library

dht DHT;

#define DHT11_PIN 4
#define IR_PIN 2

#define ON  0XFFA25D  //The value of button 1 of the remote control is 0XFFA25D
#define OFF 0XFF629D  //The value of button 2 of the remote control is 0XFF629D

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);  
IRrecv irrecv(IR_PIN);
decode_results results;

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

  lcd.init();              // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  
    ShowHumiture();
    if (irrecv.decode(&results)) 
    {
        Serial.print("value = ");
        Serial.println(results.value, HEX);

        if (results.value == ON ) //Positive motor rotation value
        {
          lcd.backlight(); //Open the backlight
            
        } 
        else if (results.value == OFF ) //Positive motor rotation value
        {
            lcd.noBacklight(); // Close the backlight
            lcd.clear();
        } 

        irrecv.resume(); // Receive the next value
  }
  delay(100);
}

void ShowHumiture()
{
   int chk = DHT.read11(DHT11_PIN);
    //Serial.println("DHT TEST PROGRAM ");
    lcd.setCursor(0,0);
    lcd.print("humi:");
    lcd.setCursor(5,0);
    lcd.print(DHT.humidity);

    lcd.setCursor(0,1);
    lcd.print("temp:");
    lcd.setCursor(5,1);
    lcd.print(DHT.temperature);

}
