#include "IRremote.h"
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 4 // Number of 2812 lamps
#define  RGB_PIN            13 // 2812 pin definition  13
#define  receiver           2  //  2

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//Creating light objects

IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'


//Variable definition
int count_R=0;
int count_G=0;
int count_B=0;
int count_key=0;
int Press=0;
int rotate=0;

//Initialization function
void setup ()
{

	pinMode(receiver,INPUT);
	
	pixels.begin(); // Initialize 2812 library functions
	pixels.show();
	
	irrecv.enableIRIn(); // Start the receiver
	
	Serial.begin(9600);
}


void loop()
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    //Serial.println(count_key); 
    irrecv.resume(); // receive the next value
  } 
	pixels.clear(); // Lighting function
	RGB();          //RGB function

}

void RGB()
{
	switch(count_key)    //Number of key presses
	{
		case 1 :         //The red light is on
			delay(10);
			count_R++;
			if(count_R>=255)count_R=255;
			count_G=0;
			count_B=0;
			pixels.setPixelColor(0, pixels.Color(count_R, 0, 0));
			pixels.setPixelColor(1, pixels.Color(count_R, 0, 0));
			pixels.setPixelColor(2, pixels.Color(count_R, 0, 0));
			pixels.setPixelColor(3, pixels.Color(count_R, 0, 0));
			pixels.show();
			break;
		case 2 :         //The green light is on
			delay(10);
			count_G++;
			if(count_G>=255)count_G=255;
			count_R=0;
			count_B=0;
			pixels.setPixelColor(0, pixels.Color(0, count_G, 0));
			pixels.setPixelColor(1, pixels.Color(0, count_G, 0));
			pixels.setPixelColor(2, pixels.Color(0, count_G, 0));
			pixels.setPixelColor(3, pixels.Color(0, count_G, 0));
			pixels.show();
			break;
		case 3 :          //The blue light is on
			delay(10);
			count_B++;
			if(count_B>=255)count_B=255;
			count_R=0;
			count_G=0;
			pixels.setPixelColor(0, pixels.Color(0, 0, count_B));
			pixels.setPixelColor(1, pixels.Color(0, 0, count_B));
			pixels.setPixelColor(2, pixels.Color(0, 0, count_B));
			pixels.setPixelColor(3, pixels.Color(0, 0, count_B));
			pixels.show();
			break;
		case 4 :          //The white light is on
			//delay(100);
			pixels.setPixelColor(0, pixels.Color(255, 255, 255));
			pixels.setPixelColor(1, pixels.Color(255, 255, 255));
			pixels.setPixelColor(2, pixels.Color(255, 255, 255));
			pixels.setPixelColor(3, pixels.Color(255, 255, 255));
			pixels.show();
			break;
		case 5 :          //Turn off the light
			pixels.setPixelColor(0, pixels.Color(0, 0, 0));
			pixels.setPixelColor(1, pixels.Color(0, 0, 0));
			pixels.setPixelColor(2, pixels.Color(0, 0, 0));
			pixels.setPixelColor(3, pixels.Color(0, 0, 0));
			pixels.show();
			break;
		case 6 :      //Red, green and blue flicker alternately
		Serial.println(rotate);		
		if(rotate<=4)
		{
			for (int j=0;j<=3;j++)
			{		
				pixels.setPixelColor(j, pixels.Color(150, 0, 0));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);
			}
			rotate++;
		}
		if((rotate>=5)&&(rotate<10))
		{
			for (int j=0;j<=3;j++)
			{		
				pixels.setPixelColor(j, pixels.Color(0, 150, 0));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);
			}
			rotate++;
		}
		if(rotate>=10)
		{
			for (int j=0;j<=3;j++)
			{		
				pixels.setPixelColor(j, pixels.Color(0, 0, 150));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);				
			}
			rotate++;
		}
		if(rotate>=15)rotate=0;
		break;
			
		default : break;
	}

   
}

void translateIR() // takes action based on IR code received
{
  switch(results.value)
  {
	case 0XFFA25D: count_key=1; break;  //Key 1 received
	case 0XFF629D: count_key=2; break;  //Key 2 received
	case 0XFFE21D: count_key=3; break;  //Key 3 received
	case 0XFF38C7:                      //Key OK received
	if(count_key==4)count_key=5;
	else count_key=4;break;
	case 0XFF02FD : count_key =6;break;  //Key 5 received
	default: break;
  }

} //END translateIR
    
