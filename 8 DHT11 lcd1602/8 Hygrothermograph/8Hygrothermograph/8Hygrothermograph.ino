#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHT11_PIN 4
dht DHT;

 // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4); 

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

   lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
}

void loop()
{
  
  ShowHumiture();
  
}


void ShowHumiture()
{
   int chk = DHT.read11(DHT11_PIN);
    //Serial.println("DHT TEST PROGRAM ");
    lcd.setCursor(0,0);
    lcd.print("humi:");
    lcd.setCursor(5,0);
    lcd.print(DHT.humidity);//Display humidity value

    lcd.setCursor(0,1);
    lcd.print("temp:");
    lcd.setCursor(5,1);
    lcd.print(DHT.temperature);//Display the temperature value

    Serial.print("humi:");
    Serial.println(DHT.humidity);
    Serial.print("temp:");
    Serial.println(DHT.temperature);

    delay(100);
}
