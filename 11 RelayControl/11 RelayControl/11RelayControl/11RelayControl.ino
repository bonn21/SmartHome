#include <IRremote.h>

#define ON  0XFFA25D  //Remote control button 1 value
#define OFF 0XFF629D  //Remote control button 2 value

#define RelayPin 7  //Relay control signal
#define FanPinA 5  //Motor Signal A
#define FanPinB 6  //Motor Signal B

IRrecv irrecv(2); //红外引脚

decode_results results;

void setup()
{
    Serial.begin(115200);
    irrecv.enableIRIn(); // Start the receiver

    pinMode(FanPinA, OUTPUT);
    pinMode(FanPinB, OUTPUT);
    pinMode(RelayPin, OUTPUT);

    digitalWrite(RelayPin, LOW);
    digitalWrite(FanPinA, LOW); 
    digitalWrite(FanPinB, LOW);
}

void loop()
{
  if (irrecv.decode(&results)) 
    {
        Serial.print("value = ");
        Serial.println(results.value, HEX);

        if (results.value == ON ) //Turn on relay
        {
           digitalWrite(RelayPin, HIGH);
           digitalWrite(FanPinA, HIGH);
           digitalWrite(FanPinB, LOW);
        } 
        else if (results.value == OFF ) //Shut off relay
        {
            digitalWrite(RelayPin, LOW);
            digitalWrite(FanPinA, LOW);
            digitalWrite(FanPinB, LOW);
        } 

        irrecv.resume(); // Receive the next value
    } 
}
