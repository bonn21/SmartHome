#include <Servo.h>

int rain;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int RainFlag = 0;   //
unsigned long currentMillis;
unsigned long time1;

void setup()
{
    Serial.begin(115200);
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    pinMode(A0, INPUT);   
}

void loop()
{
    currentMillis = millis();

    //Acquisition of sensor data
    rain = map(analogRead(A0), 0, 1023, 255, 0);
    
    Serial.print("RainData =");
    Serial.println(rain);

    //The steering gear turns to close the window in the rain
    if(rain > 130  && !RainFlag)
    {
        for ( pos = 0; pos < 180; pos++)
        {
          //'POS' is the servo motor rotation Angle
            myservo.write(pos); 
            delay(10);
        }
        RainFlag = 1;
    }
    
    //The steering gear turns without rain, open the window
    else if(rain < 130 && RainFlag)
    {
      //Wait ten seconds until it rains before opening the window
        if (currentMillis - time1 >= 1000*10)
        {
            time1 = currentMillis;       
            for ( pos = 180; pos >= 0; pos--)
            {
              //'POS' is the servo motor rotation Angle
                myservo.write(pos);              
                delay(10);
            }
            RainFlag = 0;
        }
    }
    else
    { 
       time1 = currentMillis;    
    }
   
}
