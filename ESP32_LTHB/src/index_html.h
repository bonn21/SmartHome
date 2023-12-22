#include<Arduino.h>

// chuỗi này để lưu trữ html
const char index_html[] PROGMEM = R"HoaiBac(
<!DOCTYPE html>
<html>

<head>
  <meta charset='utf-8'>
  <meta http-equiv='X-UA-Compatible' content='IE=edge'>
  <title>SmartHome - Home</title>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <link rel='stylesheet' type='text/css' media='screen' href='main.css'>
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
    integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <script src='https://kit.fontawesome.com/a076d05399.js' crossorigin='anonymous'></script>
  <script src='main.js'></script>
  <style>
    .button {
      border: none;
      border-radius: 25px;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 20px;
      /* margin: 4px 2px; */
      cursor: pointer;
      width: 160px;
      height: 80px;
      size: 32px;
    }

    .btnOn {
      background-color: #4CAF50;
    }

    /* Green */
    .btnOff {
      background-color: #008CBA;
    }

    /* Blue */

    .btn_obj {
      display: flex;
      align-items: center;
      justify-content: center;

      /* border-radius: 25px; */
    }

    .text_style {
      font-size: 48px;
      font-weight: bold;
      /* text-decoration: solid; */
    }

    .units {
      font-size: 1.2rem;
    }
  </style>
</head>

<body>
  <h1 style="text-align:center; color: blue; font-size: 48px;">SMART HOME</h1>

  <div style="display: flex; align-items: center; justify-content: center;">
    <a href="/auto">
      <button class="auto" style="margin-top: 20px;">Auto</button>
    </a>
  </div>
  
  <div style="display: flex; align-items: center; justify-content: center;  margin-top: 50px;">

    <div
      style="display: flex; height: 100px;  align-items: center; justify-content: center; width: 350px;text-align: center; margin-right: 200px;">
      <i class="fas fa-thermometer-half" style="color:red; font-size: 64px; margin-right: 10px;"></i>
      <p style="font-size: 32px;" class="text_style">Temp: </p>
      <span id="temperature" style="font-size: 32px;">%TEMPERATURE%</span>
      <sup style="font-size: 32px; font-weight: bold;">&deg;C</sup>
    </div>

    <div
      style="display: flex; height: 100px;  align-items: center; justify-content: center; width: 350px; text-align: center; margin-right: 200px;">
      <i class="fas fa-tint" style="color:#00add6; font-size: 64px; margin-right: 10px;"></i>
      <p style="font-size: 32px;" class="text_style">Humi: </p>
      <span id="humidity"  style="font-size: 32px;">%HUMIDITY%</span>
      <sup style="font-size: 32px; font-weight: bold;">&percnt;</sup>
        </div>
         
         <div
            style=" display: flex; height: 100px; align-items: center; justify-content: center; width: 350px;
        text-align: center;">

            <i class="fas fa-fire-alt" style="font-size:64px;color:orange;"></i>
            <p style="font-size: 32px;" class="text_style">Fire: </p>
            <span id="rain" style="font-size: 32px;">%GAS%</span>
            <sup style="font-size: 32px; font-weight: bold;">&percnt;</sup>
    </div>
  </div>
  <div style="display: flex; margin-top: 100px;" class="btn_obj">
    <div
      style="border-style: groove; border-radius: 25px; width: 450px; height: 400px; border-color: red; margin-right: 30px;">
      <p style="text-align: center;" class="text_style">Ban Công</p>
      <div class="btn_obj">
        <a href="/manual/balcony/open_door">
          <button class="button btnOn" style="margin-right: 20px;">Mở Cửa</button>
        </a>
      </div>
      <div class="btn_obj" style="margin-top: 20px;">
        <a href="/manual/balcony/cloes_door">
          <button class="button btnOff" style="margin-right: 20px;">Đóng Cửa</button>
        </a>
      </div>
    </div>

    <div
      style="border-style: groove; border-radius: 25px; width: 550px; height: 400px; border-color: #4CAF50; margin-right: 30px;">
      <p style="text-align: center;" class="text_style">Phòng Khách - Bếp</p>
      <div class="btn_obj">
        <a href="/manual/livingroom/ledon">
          <button class="button btnOn" style="margin-right: 10px; border-style: groove; border-color: black;">Bật
            Đèn</button>
        </a>
        
        <a href="/manual/livingroom/fanon">
          <button
            style="font-size: 20px; margin-right: 10px; border-style: groove; border-color: black; border-radius: 25px; background-color: #4CAF50; text-align: center;  width: 150px; height: 80px;">
            Bật Quạt P.Khách
          </button>
        </a>

        <a href="/manual/kitchen/fanon">
          <button
          style="font-size: 20px; margin-right: 10px; border-style: groove; border-color: black; border-radius: 25px;  background-color: #4CAF50; text-align: center; width: 150px; height: 80px;">Bật
          Quạt P.Bếp</button>
        </a>
      </div>

      <div class="btn_obj" style="margin-top: 20px;">
        <a href="/manual/livingroom/ledoff">
          <button class="button btnOff" style="margin-right: 10px; border-style: groove; border-color: black;">Tắt
            Đèn</button>
        </a>
        
        <a href="/manual/livingroom/fanoff">
          <button class="button btnOff"
          style="margin-right: 10px; border-style: groove; border-color: black; text-align: center; border-radius: 25px; width: 150px; height: 80px;">
          Tắt Quạt P.Khách
        </button>
        </a>
        
        <a href="/manual/kitchen/fanoff">
          <button class="button btnOff"
          style="margin-right: 10px; border-style: groove; border-color: black; border-radius: 25px; text-align: center; width: 150px; height: 80px;">Tắt
          Quạt P.Bếp</button>
        </a>

      </div>
    </div>

    <div
      style="border-style: groove; width: 450px; border-radius: 25px; height: 400px; border-style: groove; border-color: blue; margin-right: 30px;">
      <div>
        <p style="text-align: center;" class="text_style">Phòng Ngủ</p>
        <a href="/manual/bedroom/ledon">
          <div class="btn_obj">
            <button class="button btnOn">Bật Đèn</button>
          </div>
        </a>
        <div class="btn_obj" style="margin-top: 20px;">
          <a href="/manual/bedroom/ledoff">
            <button class="button btnOff">Tắt Đèn</button>
          </a>
        </div>
      </div>
    </div>
  </div>

  <div class="btn_obj">
    <div
      style="border-style: groove; width: 450px; border-radius: 25px; height: 400px; border-color: blue; margin-top: 30px; margin-left: -30px;">
      <p style="text-align: center;" class="text_style">Vườn</p>
      <div class="btn_obj">
        <a href="/manual/garden/pumpon">
          <button class="button btnOn" style="width: 180px; height: 80px; background-color: #4CAF50; font-size: 20px; border-radius: 25px;">
            Bật Máy Bơm</button>
        </a>
      </div>
      <div class="btn_obj" style="margin: 20px;">
        <a href="/manual/garden/pumpoff">
          <button class="button btnOff" style="width: 180px; height: 80px; font-size: 20px; border-radius: 25px;">Tắt
            Máy Bơm</button>
        </a>
      </div>
    </div>
  </div>

</body>

</html>)HoaiBac";
