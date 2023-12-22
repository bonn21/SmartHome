#define  LED            13  // Define output pin
#define  KEY            12  // Define input pin

int KEY_BIT=0;              //Define status bits
int LED_BIT=0;              //Define status bits

void setup ()   //Function initialization
{

	pinMode(LED,OUTPUT);  //Define output
	pinMode(KEY,INPUT);   //Define input

}


void loop()   //Principal function
{
	if((digitalRead(KEY))&&(KEY_BIT)) //Judge whether the key is pressed
	{
		KEY_BIT=0;
		if(LED_BIT==0)LED_BIT=1;
		else LED_BIT=0;
	}
	else
	{
		delay(20);
		if(!digitalRead(KEY))
		{
			KEY_BIT=1;
		}
	}
	
	if(LED_BIT) digitalWrite(LED,HIGH); //The output of single chip microcomputer is high
	else digitalWrite(LED,LOW);  //Low output of single chip microcomputer


}
































