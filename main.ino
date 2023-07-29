#include <M5Stack.h>
#include "ClosedCube_TCA9548A.h"
#include "VL53L0X.h"
#include "Wire.h"
#include "BaseX.h"
#include <Unit_Sonic.h>
#include "ClosedCube_TCA9548A.h"
#include "distanceSensor.h" //順番
#include "Adafruit_TCS34725.h"

#define FRONT 2
#define X_LOCAL 100
#define Y_LOCAL 35
#define X_OFFSET 160
#define Y_OFFSET 34

BASE_X base_x          = BASE_X();
int32_t position_point = 0;
int8_t mode            = NORMAL_MODE;
uint8_t max_speed      = 20;
int8_t speed_point     = 0;
SONIC_I2C sensor;

int distRR;
int distRF;
int distFR;
int distFL;

#define PaHub_I2C_ADDRESS 0x70


void setup()
{
  
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);

  // 画面初期設定
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("START");
  
  // シリアル通信機能の設定
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  M5.begin();
  M5.Power.begin();
  tca9548a.address(PaHub_I2C_ADDRESS); // Set the I2C address.  设置I2C地址
  // M5.Lcd.setTextFont(4);
  // M5.Lcd.setCursor(70, 0, 4);
  // M5.Lcd.setTextColor(YELLOW, TFT_BLACK);
  // M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  Serial.begin (115200);
  sensor.begin();
  

  // // Power chipがgpio21, gpio22, I2Cにつながれたデバイスに接続される。
  // //バッテリー動作の場合はこの関数を読んでください（バッテリーの電圧を調べるらしい）

  Wire.begin(); // I2C通信を開始する
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);





  //                         // M5.Lcd.print("ok2");

    M5.begin(true, false, true, true);
  M5.Power.begin();

  //Serial.begin(115200);  
  delay(100);
  //Serial.println("test");
  tca9548a.address(PaHub_I2C_ADDRESS);

  M5.Lcd.fillRect(0, 0, 319, 239, BLACK);
  M5.Lcd.setCursor(0, 50, 4);

  M5.Lcd.fillRect(0, 0, 319, 35, WHITE);
  M5.Lcd.println("Press B to start");


  M5.Lcd.fillRect(0, 0, 319, 239, WHITE);

}

void getsonic()
{


  M5.update();
  M5.Lcd.fillRect(0, 35, 319, 189, BLACK);

  
  distFL = getdistance(1);  
  if (distFL == 2000) distFL = getdistance(1);
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.print("FrontLeft:  ");M5.Lcd.println(distFL);
  
  distFR = getdistance(2);
  if (distFR == 2000) distFR = getdistance(2);
  M5.Lcd.setCursor(0, 80);
  M5.Lcd.print("FrontRight: ");M5.Lcd.println(distFR);

  distRF = getdistance(3);
  if (distRF == 2000) distRF = getdistance(3);
  M5.Lcd.setCursor(0, 120);
  M5.Lcd.print("RightFront: ");M5.Lcd.println(distRF);

  distRR = getdistance(0);
  if (distRR == 2000) distRR = getdistance(0);
  M5.Lcd.setCursor(0, 160);
  M5.Lcd.print("RightRear:  ");M5.Lcd.println(distRR);

/*
  Serial.print(distFL);
  Serial.print(',');
  Serial.print(distFR);
  Serial.print(',');
  Serial.print(distRF);
  Serial.print(',');
  Serial.print(distRR);
  Serial.println(' ');
*/

  

  //描画
  bool RWall = !(distRF > 250 && distRR > 250);  // true if Right wall exists 
  bool FWall = !(distFL > 200 && distFR > 200);          // true if Front wall

  // if (RWall){
  //   M5.Lcd.drawLine(300,50,300,150,WHITE);
  // }else{
  //   M5.Lcd.drawLine(300,50,300,150,BLACK);
  // }
  // if (FWall){
  //   M5.Lcd.drawLine(200,50,300,50,WHITE);
  // }else{
  //   M5.Lcd.drawLine(200,50,300,50,BLACK);
  // }

  // delay(500);
}
void loop()
{
  M5.update();

  getsonic();
}

  print()
