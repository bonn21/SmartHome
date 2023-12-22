#define  LED            11  // Define output pin

int i=0;      //Data type definition
int j=0;

void setup ()   //Function initialization
{

	pinMode(LED,OUTPUT);  //Define output

}


void loop()   //Principal function
{
	for(i=0;i<=255;i++)   //Gradually bright
	{
		analogWrite(LED,i);
		delay (10);
	}
	for(j=255;j>=0;j--)   //Die out
	{
		analogWrite(LED,j);
		delay (10);
	}

}
































