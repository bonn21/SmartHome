
#define  LED 13  // Define output pin

void setup ()   //Function initialization
{

	pinMode(LED,OUTPUT);  //Define output

}


void loop()   //Principal function
{

		digitalWrite(LED,HIGH); //The output of single chip microcomputer is high
		delay(500);             //delayed
		digitalWrite(LED,LOW);  //Low output of single chip microcomputer
		delay(500);             ////delayed


}
