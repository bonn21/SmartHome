// Thư viện
#include <Arduino.h> 
#include <Wire.h> 
#include <ArduinoJson.h> 
#include <ESP32Servo.h> 
#include <DHT.h> 
#include <DHT_U.h>
#include <user_rfid.h> 
#include <network.h>
#include "ESPAsyncWebServer.h" 
#include "SPIFFS.h"
#include <index_html.h>

// Setup chân
#define RL1         27 //RL là relay
#define RL2         16
#define RL3         26
#define RL4         14
#define RL5         33
#define RL6         25
#define RLON        LOW
#define RLOFF       HIGH
#define PIN_MQ2     32
#define SERVO1      12
#define SERVO2      13
#define PIN_RAIN    34
#define PIN_SOILD   35
#define IR1         22
#define IR2         17
#define POS_OPEN    0
#define POS_CLOSE   45
#define DHT11_DATA  15
#define DHTTYPE     DHT11 // DHT 22 (AM2302)
sensor_t sensor;
sensors_event_t event;

AsyncWebServer server(80); // port 80

// Tạo biến dữ liệu cho các cảm biến
DHT_Unified dht(DHT11_DATA, DHTTYPE);
uint8_t _dht_temp = 0; // nhiệt độ
uint8_t _dht_humi = 0; // độ ẩm
uint8_t _soil = 0; // độ ẩm đất
uint8_t mode_auto = 1; // chế độ tự động

Servo myservo1, myservo2;

// Setup chạng thái của các thiết bị
typedef struct
{
  // 0: tắt, 1: bật
  uint8_t balcony_led = 0; 
  uint8_t balcony_door = 0; 
  uint8_t living_led = 0;
  uint8_t living_fan = 0;
  uint8_t kitchen_fan = 0;
  uint8_t bedroom_led = 0;
  uint8_t garden_pump = 0;
} Dev;

int value_mq2 = 0;
// Khai báo timer0
hw_timer_t *Timer0_Cfg = NULL; 
uint32_t Freq = 0; //
int cnt = 0; //
uint16_t cnt_IR1 = 0, cnt_IR2 = 0; 

uint8_t flag_stt = 0; 
// Hàm ngắt
void IRAM_ATTR Blink_Led() 
{
  flag_stt = 1; 
}
// khai báo các hàm
void Interrupt_Init(); 
void Web_Control(); 
void Operate_System();
String MQ2_Read();
String Rain_Read();
String readDHTTemperature();
String Soild_Read();
String readDHTHumidity();
int Soil_Read(); 

Dev deviec; 
void Dht11_Init();
void Gpio_Init();
// các biến cho các thiết bị
uint8_t flag_check_rf = 0; 
uint8_t flag_control_IR = 0;
uint8_t flag_control_fan_living = 0, flag_control_fan_kitchen = 0, flag_control_pump = 0;
uint8_t mode_select_door = 0;
uint8_t mode_control_device = 0;
uint8_t control_IR1 = 0, control_IR2 = 0;
uint8_t control_fan_living = 0, control_fan_kitchen = 0;
// kiểm tra các biến được chuyền vào
String processor(const String &var) 
{
  if (var == "TEMPERATURE") 
  {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY") 
  {
    return readDHTHumidity();
  }
  else if (var == "GAS")
  {
    return MQ2_Read();
  }
  return String();
}
// hàm chính
void setup()
{
  Serial.begin(115200); 
  Interrupt_Init(); 
  Gpio_Init(); 
  Dht11_Init(); 
  myservo1.attach(SERVO1);
  myservo2.attach(SERVO2);
  RFID_Init();
  if (!SPIFFS.begin(true)) // kiểm tra trang thái hoạt động của SPIFFS
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Wifi_Setup();
  Web_Control();
}
// vòng lặp
void loop()
{
  if (flag_stt) 
  {
    flag_check_rf = 1; 
    flag_control_IR = 1; 
    cnt++;
    flag_stt = 0;
  }

  if (flag_check_rf)
  {
    RFID_Check();
    flag_check_rf = 0;
  }
  if (flag_open_door == 1)
  {
    mode_select_door = 1;
    if (cnt % 50 == 0)
    {
      cnt = 0;
      flag_open_door = 0;
      mode_select_door = 0;
      myservo1.write(POS_CLOSE);
      Serial.println("CLOSE");
    }
    else
    {
      myservo1.write(POS_OPEN);
      Serial.println("OPEN");
    }
  }

  if (mode_auto)
  {
    if (flag_control_IR)
    {
      if (!digitalRead(IR1))
      {
        control_IR1 = 1;
        // cnt_IR1 = 0;
      }
      if (!digitalRead(IR2))
      {
        // cnt_IR2 = 0;
        control_IR2 = 1;
      }

      if (control_IR1)
      {
        // cnt_IR1 = 0;
        if (cnt % 50 == 0)
        {
          cnt = 0;
          control_IR1 = 0;
          flag_control_IR = 0;
          digitalWrite(RL5, RLOFF);
          Serial.println("OFF RL5");
        }
        else
        {
          digitalWrite(RL5, RLON);
          Serial.println("ON RL5");
        }
      }

      if (control_IR2)
      {
        // cnt_IR2 = 0;
        if (cnt % 50 == 0)
        {
          cnt = 0;
          control_IR2 = 0;
          flag_control_IR = 0;
          digitalWrite(RL2, RLOFF);
          Serial.println("OFF RL2");
        }
        else
        {
          digitalWrite(RL2, RLON);
          Serial.println("ON RL2");
        }
      }
    }

    if (flag_control_fan_kitchen)
    {
      if (value_mq2 >= 20)
      { // cảnh bảo khí gas trong phòng bếp
        if(cnt % 50 != 0){
          digitalWrite(RL4, RLON);
        }
      }
      else
      {
        if(cnt % 50 == 0){
          digitalWrite(RL4, RLOFF);
          Serial.println("MQ2 OFF");
          flag_control_fan_kitchen = 0;
          cnt = 0;
        }
      }
    }
    if (flag_control_fan_living)
    {
      if (_dht_temp >= 29)
      { 
        if(cnt % 50 != 0){
          digitalWrite(RL3, RLON);
        }
      }
      else
      {
        if(cnt % 50 == 0){
          digitalWrite(RL3, RLOFF);
          Serial.println("Fan Living OFF");
          flag_control_fan_living = 0;
          cnt = 0;
        }
      }
    }

    if (flag_control_pump)
    {
      if (_soil <= 60)
      { //
        if(cnt % 50 != 0){
          digitalWrite(RL6, RLON);
        }
      }
      else
      {
        if(cnt % 50 == 0){
          digitalWrite(RL6, RLOFF);
          Serial.println("pump OFF");
          flag_control_pump = 0;
          cnt = 0;
        }
      }
    }
  }

  Operate_System();
  delay(20);
}

void Interrupt_Init()
{
  Timer0_Cfg = timerBegin(0, 8000, true);
  timerAttachInterrupt(Timer0_Cfg, &Blink_Led, true);
  timerAlarmWrite(Timer0_Cfg, 1000, true);
  timerAlarmEnable(Timer0_Cfg);
}

void Gpio_Init()
{
  pinMode(RL1, OUTPUT);
  pinMode(RL2, OUTPUT);
  pinMode(RL3, OUTPUT);
  pinMode(RL4, OUTPUT);
  pinMode(RL5, OUTPUT);
  pinMode(RL6, OUTPUT);
  digitalWrite(RL1, RLOFF);
  digitalWrite(RL2, RLOFF);
  digitalWrite(RL3, RLOFF);
  digitalWrite(RL4, RLOFF);
  digitalWrite(RL5, RLOFF);
  digitalWrite(RL6, RLOFF);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
}

void Dht11_Init()
{
  dht.begin();
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
}

String MQ2_Read()
{
  int value = 0;
  for (int i = 0; i < 10; i++)
  {
    value += analogRead(PIN_MQ2);
  }
  value_mq2 = value;
  return String(map(value / 10, 0, 4095, 0, 100));
}

String Rain_Read()
{
  return String(digitalRead(PIN_RAIN));
}

String Soild_Read()
{
  return String(digitalRead(PIN_SOILD));
}

String readDHTTemperature()
{
  dht.temperature().getEvent(&event);
  _dht_temp = event.temperature;
  return String(_dht_temp);
}

String readDHTHumidity() // đọc độ ẩm và trả lại dưới dạng chuỗi
{
  dht.humidity().getEvent(&event);
  _dht_humi = event.relative_humidity;
  return String(_dht_humi);
}

int Soil_Read()
{
  int value = 0;
  for (int i = 0; i < 10; i++)
  {
    value += analogRead(PIN_SOILD);
  }
  _soil = value / 10;
  return map(value / 10, 0, 4095, 0, 100);
}

/***********************************************************************/

void Web_Control()
{
  // đường dẫn khi mở web, ấn f12 để hiểu hơn về hàm
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // flag_manual = 0; // mặc định quạt chạy ở chế độ auto
    request->send_P(200, "text/html", index_html, processor); });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });

  // lấy thông tin độ ẩm
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDHTHumidity().c_str()); });

  // Mode auto
  server.on("/auto", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    mode_auto = 1;
    request->send_P(200, "text/html", index_html, processor); });

  // khu vực hành lang, ban công
  server.on("/manual/balcony/ledon", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.balcony_led = 1;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/balcony/ledoff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.balcony_led = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/balcony/open_door", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.balcony_door = 1;
    mode_select_door = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/balcony/cloes_door", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.balcony_door = 0;
    mode_select_door = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });

  // Khu vực phòng khách - bếp
  server.on("/manual/livingroom/ledon", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.living_led = 1;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/livingroom/ledoff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.living_led = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  //------------------------------
  server.on("/manual/livingroom/fanon", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.living_fan = 1;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/livingroom/fanoff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.living_fan = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  //------------------------------
  server.on("/manual/kitchen/fanon", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.kitchen_fan = 1;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/kitchen/fanoff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.kitchen_fan = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });

  // khu vực phòng ngủ
  server.on("/manual/bedroom/ledon", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.bedroom_led = 1;
    mode_control_device = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/bedroom/ledoff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.bedroom_led = 0;
    mode_control_device = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });

  // khu vực vườn
  server.on("/manual/garden/pumpon", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.garden_pump = 1;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.on("/manual/garden/pumpoff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    deviec.garden_pump = 0;
    mode_auto = 0;
    request->send_P(200, "text/html", index_html, processor); });
  server.begin();
}

void Operate_System()
{
  // khu vực hành lang
  if (deviec.balcony_led)
  {
    digitalWrite(RL1, RLON);
  }
  else
  {
    digitalWrite(RL1, RLOFF);
  }
  if (deviec.balcony_door == 1 && mode_select_door == 0)
  {
    myservo1.write(POS_OPEN);
  }
  else if (deviec.balcony_door == 0 && mode_select_door == 0)
  {
    mode_select_door = 1;
    myservo1.write(POS_CLOSE);
  }

  // khu vực phòng khách
  if (deviec.living_led == 1 && mode_auto == 0)
  {
    digitalWrite(RL2, RLON);
  }
  else if (deviec.living_led == 0 && mode_auto == 0)
  {
    digitalWrite(RL2, RLOFF);
  }
  if (deviec.living_fan == 1 && mode_auto == 0)
  {
    digitalWrite(RL3, RLON);
  }
  else if (deviec.living_fan == 0 && mode_auto == 0)
  {
    digitalWrite(RL3, RLOFF);
  }
  if (deviec.kitchen_fan == 1 && mode_auto == 0)
  {
    digitalWrite(RL4, RLON);
  }
  else if (deviec.kitchen_fan == 0 && mode_auto == 0)
  {
    digitalWrite(RL4, RLOFF);
  }

  // Khu vực phòng ngủ
  if (deviec.bedroom_led == 1 && mode_auto == 0)
  {
    digitalWrite(RL5, RLON);
  }
  else if (deviec.bedroom_led == 0 && mode_auto == 0)
  {
    digitalWrite(RL5, RLOFF);
  }

  // khu vực vườn
  if (deviec.garden_pump == 1 && mode_auto == 0)
  {
    digitalWrite(RL6, RLON);
  }
  else if (deviec.garden_pump == 0 && mode_auto == 0)
  {
    digitalWrite(RL6, RLOFF);
  }
}