#include <IRremote.h>

#define Level0  0XFF9867  //Button 0
#define Level1  0XFFA25D  //Button 1
#define Level2  0XFF629D  //Button 2
#define Level3  0XFFE21D  //Button 3
#define Level4  0XFF22DD  //Button 4
#define Level5  0XFF02FD  //Button 5
#define Level6  0XFFC23D  //Button 6
#define Level7  0XFFE01F  //Button 7
#define Level8  0XFFA857  //Button 8
#define Level9  0XFF906F  //Button 9

#define SpeedUp   0XFF18E7//
#define SlowDown  0XFF4AB5//
#define OFF_ON    0XFF38C7//Button  OK


#define FanPinA 5  //
#define FanPinB 6  //

IRrecv irrecv(2);

int FanSpeed = 0;
decode_results results;
bool flag = 0;

void setup()
{
    Serial.begin(115200);
    irrecv.enableIRIn(); // Start the receiver

    pinMode(FanPinA, OUTPUT);
    pinMode(FanPinB, OUTPUT);
}

void loop()
{
    if (irrecv.decode(&results)) 
    {
        Serial.print("value = ");
        Serial.println(results.value, HEX);

        switch (results.value)
        {
        case  Level0:
           FanSpeed = 0;
            break;

        case  Level1:
            FanSpeed = 50;
            break;

        case  Level2:
        FanSpeed = 75;
            break;

        case  Level3:
        FanSpeed = 95;
            break;
       
        FanSpeed = 120;
        case  Level4:
        
        FanSpeed = 145;
            break;

        case  Level5:
        FanSpeed = 165;
            break;

        case  Level6:
        FanSpeed = 185;
             break;
        case  Level7:
        FanSpeed = 205;
            break;

        case  Level8:
       FanSpeed = 220;
            break;
            FanSpeed = 240;
        
        case  Level9:
        FanSpeed = 255;
            break;
            
            case  SpeedUp:
            FanSpeed += 25;
            if(FanSpeed > 255 )
            {
                FanSpeed = 255;
            }
            break;

            case  SlowDown:
            FanSpeed -= 25;
            if(FanSpeed < 0 )
            {
                FanSpeed = 0;
            }
            break; 

        default:
            break;
                
    }
        Serial.print("speed = ");
        Serial.println(FanSpeed);
        analogWrite(FanPinA,0);
        analogWrite(FanPinB,FanSpeed);
        
       
        irrecv.resume(); // Receive the next value
  } 
  
}
