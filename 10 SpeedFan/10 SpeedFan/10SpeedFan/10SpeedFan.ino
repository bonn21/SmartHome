/*
Speed fan experiment, slowly accelerate, slow down.
*/

#define FanPinA 5  //L9110 1A
#define FanPinB 6  //L9110 1B

void setup()
{
    Serial.begin(115200);
    
    //Configure the pin mode
    pinMode(FanPinA, OUTPUT);
    pinMode(FanPinB, OUTPUT);
}

void loop()
{
    //Speed increased from 30 to 255
    for (int i = 30; i <= 255; i++)
    {
        Serial.print("speed = ");
        Serial.println(i);
        analogWrite(FanPinA,i);
        analogWrite(FanPinB,0);
        delay(50);
    } 
    //Speed reduced from 255 to 30
    for (int i = 255; i >= 30; i--)
    {
        Serial.print("speed = ");
        Serial.println(i);
        analogWrite(FanPinA,i);
        analogWrite(FanPinB,0);
        delay(50);
    } 
}
