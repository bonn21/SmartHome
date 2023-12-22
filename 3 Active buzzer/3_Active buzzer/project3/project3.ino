#define  buzzer            13  // Define buzzer output pin


void setup ()
{

	pinMode(buzzer,OUTPUT); //Set pin to output
}


void loop()
{

		digitalWrite(buzzer,HIGH);  //The output is high and the buzzer rings
		delay(500);
		digitalWrite(buzzer,LOW);  //Low output buzzer stops
		delay(500);


}
































