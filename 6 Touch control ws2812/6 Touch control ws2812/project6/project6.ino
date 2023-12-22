#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 4 // Number of 2812 lamps
#define  RGB_PIN  13  // 2812 pin definition 13 
#define  KEY_1   12   // Key pin definition 12

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//Creating light objects


//Variable definition
int count_R=0;
int count_G=0;
int count_B=0;
int count_key=0;
int Press=0;

//Initialization function
void setup ()
{

	pinMode(KEY_1,INPUT);
	
	pixels.begin(); // Initialize 2812 library functions
	pixels.show();
	
	Serial.begin(9600);
}


void loop()
{
	pixels.clear(); // Lighting function
	KEY();          //Key function
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
      case 4 :          //The blue light is on
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.show();
      break;
     
		default : break;
	}

   
}

void KEY()   //Key function
{
	int i=digitalRead(KEY_1);
	Serial.println(count_key);
	if(i)
	{
		delay(20);
		if(i)Press=1;   //Press the sign
	}
	
	i=digitalRead(KEY_1);
	if((i==0) &&(Press==1))   //Let go detection
	{
		Press=0;
		count_key++;
		if(count_key==5)count_key=1;
	}
}
    
