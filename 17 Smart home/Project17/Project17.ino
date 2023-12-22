#include <Adafruit_NeoPixel.h>  //RGB libraries
#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>          //Infrared libraries
#include <SPI.h> 
#include <RFID.h>
#include <Servo.h>    // Servo libraries
RFID rfid(10, 9);       //D10: the pin of the tag reader SDA. D9: Label reader RST pin
unsigned char status; 
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

String accessGranted [2] = {"33131497105", "96111131033"};  //Rfid serial number authorized access
int accessGrantedSize = 2;                                //Number of serial numbers
Servo lockServo;                //Locking mechanism servo
int lockPos = 0;               //Lock position limitation
int unlockPos =90;             //Limit of unlock position
boolean locked = true;
int rain;     
Servo myservo;  // Create a servo object to control the servo
int pos = 0;    // Variables store the servo position
int RainFlag = 0;   

unsigned long currentMillis;
unsigned long time1;

dht DHT;
#define NUMPIXELS      4          //Number of LED lamp beads  LED灯珠数量4个
#define  RGB_PIN       5          // RGB module Arduino output control signal pin RGB模块Arduino输出控制信号引脚  5
#define  LED_PIN       8          //LED module Arduino output control signal pin   LED模块Arduino输出控制信号引脚 8
#define  KEY_1         A6         //Touch button Arduino output control signal pin A6  触摸按钮Arduino输出控制信号引脚A6
#define  KEY_2         A7         //Yellow button module Arduino output control signal pin 黄色按钮模块Arduino输出控制信号引脚A7
#define  buzzer        15         //Buzzer module Arduino output control signal pin  蜂鸣器模块Arduino输出控制信号引脚 A1
#define  smokeA0       A0         //MQ-2 gas sensor Arduino output control signal pin MQ-2气体传感器Arduino输出控制信号引脚A0
#define DHT11_PIN      4          //DHT11 temperature and humidity Arduino output control signal pin  DHT11温湿度Arduino输出控制信号引脚 4
#define IR_PIN         2          //Infrared receiver Arduino output control signal pin  红外接收器Arduino输出控制信号引脚 2
#define RelayPin       16         //Relay module Arduino output control signal pin     继电器模块Arduino输出控制信号引脚 A2
#define FanPinA        7          //Fan motor module Arduino output control signal pin  风扇电机模块Arduino输出控制信号引脚 7
#define Rain           A3         //Raindrop sensor Arduino output control signal pin 雨滴传感器Arduino输出控制信号引脚 A3

#define KEY11 16753245    //Control LCD to turn on the backlight 控制LCD打开背光
#define KEY22 16736925    //Control LCD to turn off backlight  控制LCD关闭背光
#define KEY33 16769565    //Control fan start  控制风扇机启动
#define KEY44 16720605    //Control fan off  控制风扇关闭
#define KEY55 16712445    //Control window opening  窗口
#define KEY66 16761405    //Control window closing  窗口关闭
#define KEY77 16769055    //Control gate open  控制门打开
#define KEY88 16754775    //Control door closing  关门时间控制装置
#define KEY00 16750695    //Conrrol open automation  控制开式自动化
#define KEY1 16718055     //Open white RGB  开白RGB
#define KEY2 16730805     //Open red RGB  开红RGB
#define KEY3 16716015     //Open green RGB 开绿RGB
#define KEY4 16734885     //Open blue RGB 开蓝RGB
#define KEY5 16753245     //Closing RGB  关闭RGB

//Set the LCD address to 0x27 to display in 16 characters and 2 lines  设置LCD地址为0x27，显示16个字符，2行

LiquidCrystal_I2C lcd(0x27,20,4);  
IRrecv irrecv(IR_PIN);
decode_results results;
Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//Create light objects

//Variable definitions
int count_R=0;
int count_G=0;
int count_B=0;
int count_key=0;
int Press=0;            //Defines key flag bits
int rotate=0;
int KEY_BIT=0;            //Defining status bits
int LED_BIT=0;            //Defining status bits
int sensorThres = 80;     //Set the gas concentration range
int color_mod = 0;      
int door = 0;

void setup() {
  // put your setup code here, to run once:
  	pinMode(KEY_1,INPUT);  //Touch definition input
	pinMode(KEY_2,INPUT);  //Key definition input
  pinMode(LED_PIN,OUTPUT);//LED
	pixels.begin(); //Initialize 2812 library functions
	pixels.show();
  pinMode(buzzer, OUTPUT); //The buzzer defines the output
  pinMode(smokeA0, INPUT); //Smoke definition input
  pinMode(FanPinA, OUTPUT);//fan
  pinMode(RelayPin,OUTPUT);//Relay module

  myservo.attach(6);  // Connect the servo on pin 6 to the servo object 将伺服引脚6连接到伺服对象(定义窗户舵机引脚6)
  pinMode(Rain, INPUT);     
  digitalWrite(RelayPin, LOW);
  digitalWrite(FanPinA, HIGH); 
  
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

  lcd.init();   // Initialized liquid crystal
  // Print message to LCD
  lcd.backlight();
  irrecv.enableIRIn(); // Starting receiver
	Serial.begin(9600);
  SPI.begin();            //Start SPI communication with the reader
  rfid.init();            //initialize 
  lockServo.attach(3);  //Connect servo pin 3 to the servo object 将伺服引脚3连接到伺服对象(定义门舵机引脚3)
  pinMode(Rain, INPUT);     
  lockServo.write(lockPos);         //Move the servo to the locked position
  Serial.println("Place card/tag near reader...");
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.clear(); // Backlight
  KEY();            //The key function
	RGB();           //RGB 
  LED();           //LED
  rainwater();    //Raindrop sensor
  RD();           //RFID
  smoke();
  LCD();
}

//The touch key initializes the function

void KEY()   //Key Function
{
  int i=analogRead(KEY_1);
  //Serial.println(color_mod);
  if(i>100)
  {
    Press=1;//Press the logo
  }
  if((i<=100)&&(Press==1))   //Check whether the key is really pressed
  {
    Press=0;
    color_mod++;
    if(color_mod > 5)color_mod=1;  
  } 
}

//LED function
void LED()
{
  if((analogRead(KEY_2))&&(KEY_BIT)) //Determine if the key is pressed
  {
    KEY_BIT=0;
    if(LED_BIT==0)LED_BIT=1;
    else LED_BIT=0;
  }
  else
  {
    delay(20);
    if(!analogRead(KEY_2))
    {
      KEY_BIT=1;
    }
  }
  
if(LED_BIT)digitalWrite(LED_PIN,HIGH);//MCU output high level
else digitalWrite(LED_PIN,LOW);      //MCU output low level
}

//RGB function  
void RGB()
{
  switch(color_mod)    //Number of keys 
  {
    case 1 :            //The red light is on.

      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      pixels.show();
      break;
    case 2 :              //green light flashes

      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.setPixelColor(1, pixels.Color(0, 255, 0));
      pixels.setPixelColor(2, pixels.Color(0, 255, 0));
      pixels.setPixelColor(3, pixels.Color(0, 255, 0));
      pixels.show();
      break;
    case 3 :               //The blue lights

      pixels.setPixelColor(0, pixels.Color(0, 0, 255));
      pixels.setPixelColor(1, pixels.Color(0, 0, 255));
      pixels.setPixelColor(2, pixels.Color(0, 0, 255));
      pixels.setPixelColor(3, pixels.Color(0, 0, 255));
      pixels.show();
      break;
      case 4 :           //bright white light
         //delay(100);
         pixels.setPixelColor(0, pixels.Color(255, 255, 255));
         pixels.setPixelColor(1, pixels.Color(255, 255, 255));
         pixels.setPixelColor(2, pixels.Color(255, 255, 255));
         pixels.setPixelColor(3, pixels.Color(255, 255, 255));
         pixels.show();
          break;
       case 5 :        // Turn our the lights
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.show();
      break;
     default : break;
  }
}

void rainwater()
 {
    //Sensor data collection
    rain = map(analogRead(Rain), 0, 1023, 255, 0);
    
    // Serial.print("RainData =");
    // Serial.println(rain);
    if(RainFlag == 1){
    if(rain > 130){ 
        myservo.write(130); //close the door
        // results.value == 16,711,680;
      }else{
         myservo.write(10);  //open the door
      }
    }
}

//RFID

void RD()
{
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  { 
    Serial.println("Card found"); 
    String temp = "";                             //A temporary variable that stores the READ RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read the label serial number
    { 
      Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                 //Record and display the label serial number
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      Serial.println (temp);
      checkAccess (temp);     //Check whether the identified label is allowed to be opened
    } 
    rfid.selectTag(str); //Lock the card to prevent repeated reading, removing the line will cause the sketch to read the card continuously
  }
  rfid.halt(); 
}

void checkAccess (String temp)    //The function checks whether a registered tag is accessible
{
  boolean granted = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)    //Iterates over all tag ID numbers registered in the group
  {
    if(accessGranted[i] == temp)            //If a label is found, then open/close the lock
    {
      Serial.println ("Access Granted");
      
      granted = true;
      if (locked == true)         //If the lock is closed, open it
      {
          lockServo.write(unlockPos);
          locked = false;
      }
      else if (locked == false)   //If the lock is open, close it
      {
          lockServo.write(lockPos);
          locked = true;
      }
    }
  }
  if (granted == false)     //If no tags are found
  {
    Serial.println ("Access Denied");
  }
}

//Mq-2 gas function

void smoke()
{
   int analogSensor = analogRead(smokeA0); //Assign the check value of the MQ-2 sensor to the ALOG sensor variable

  // Serial.print("Pin A0: ");
   Serial.println(analogSensor);
  //Check if the threshold has been reached
  if (analogSensor > sensorThres)
  {
   digitalWrite(buzzer,HIGH); //The output is high, the buzzer sounds
   delay(500);
   digitalWrite(RelayPin,HIGH);  //High output; Relay start
   digitalWrite(FanPinA, HIGH);  //The fan start
   delay(1000);
   digitalWrite(buzzer,LOW);  //The low output buzzer stops
   delay(500); 
   digitalWrite(RelayPin,LOW);  //The output is low; Relay off
   digitalWrite(FanPinA, LOW);   //Fan stop
   delay(1000);
  }
}

//Infrared control LCD 1602, fan, door and window, LED function

void LCD()
{
   translateIR();  //Infrared controls RGB functions
   ShowHumiture();
    if (irrecv.decode(&results)) 
    {
        //Serial.print("value = ");
        //Serial.println(results.value, HEX);
     
        if (results.value == KEY11 )     // LCD boot
        {
          lcd.backlight(); //Turn on the backlight
            
        } 
        else if (results.value == KEY22 ) //LCD OFF
        {
            lcd.noBacklight(); // Turn off the backlight
            lcd.clear();
        } 
          if (results.value == KEY33 ) //Switch on and activate the relay
        {
           digitalWrite(RelayPin, HIGH);
           //digitalWrite(FanPinA, HIGH);
           
        } 
        else if (results.value == KEY44 ) // Disconnect the relay
        {
            digitalWrite(RelayPin, LOW);
            //digitalWrite(FanPinA, LOW);
             
        } 

       if (results.value == KEY55 )   //Open Window
        {
          RainFlag = 0;
          myservo.write(10);
          delay(15);
           
        } 
        else if (results.value == KEY66 ) //close windows
        {
          RainFlag = 0;
          myservo.write(130);  //Change the degree of rotation of the steering gear
          delay(15);
             
        }    

        if (results.value == KEY77 )         //Open the door
          {   lockServo.write(0);
              delay(15);
           }
       else if (results.value == KEY88 )     //Close the gate
       {
          lockServo.write(90);
          delay(15);
       }

        if (results.value == KEY00 )         //Conrrol open automation
        {
              RainFlag = 1;
        }

        irrecv.resume(); // Receive the next value
      }
      delay(100);
 }

//Display temperature
void ShowHumiture()
{
   int chk = DHT.read11(DHT11_PIN);
    //Serial.println("DHT TEST PROGRAM ");
    lcd.setCursor(0,0);  
    lcd.print("humi:");
    lcd.setCursor(5,0);
    lcd.print(DHT.humidity);

    lcd.setCursor(0,1); 
    lcd.print("temp:");
    lcd.setCursor(5,1);
    lcd.print(DHT.temperature);
}
//Infrared control RGB 
void translateIR() // Act on received IR code
{
  switch(results.value)
  {
  case 0xFF4AB5:    color_mod=1; break;   //Red
  case 0xFF10EF:    color_mod=2; break;   //green
  case 0xFF5AA5:    color_mod=3; break;   //blue
  case 0xFF18E7:    color_mod=4; break;   //white 
  case 0XFF38C7:    color_mod=5; break;   //Hit the OK button to turn off the lights
  default: break;
  }
} 
