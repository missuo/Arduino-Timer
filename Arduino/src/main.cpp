#include <Arduino.h>

/*
 * A multifunctional stopwatch based on Arduino
 * Author: Vincent Young From Computer Science & Technology 2019
 * Latest Update: Dec 11, 2021
 */

#include <SoftwareSerial.h>    // Import SoftwareSerial
#include <LiquidCrystal_I2C.h> // Import I2C

SoftwareSerial mySerial(4, 3); // RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
      lcd.init();         // 初始化LCD
      lcd.backlight();   // 设置LCD背景等亮
      // delay(50);
      Serial.begin(9600); // 串口监视器波特率
      Serial.println("Welcome");
      // delay(50);
      
      mySerial.begin(9600); // 蓝牙通讯波特率
      delay(5000);
      /*
      // 初始化蓝牙
      mySerial.print("AT\r\n"); // 进入AT指令
      delay(1000);
      mySerial.print("AT+RESET\r\n");   // 重置模块
      delay(1000);
      mySerial.print("AT+VERSION\r\n");   // 查看模块版本
      delay(1000);
      mySerial.print("AT+PIN6666\r\n");   // 蓝牙密码设置长度至少为4位数
      delay(1000);
      mySerial.print("AT+NAMEVINCENT\r\n");//命名模块名
      delay(1000);        
      mySerial.print("AT+BAUD4\r\n"); //设置波特率9600
      */

      lcd.clear();
      lcd.print("TIMER READY");
      lcd.setCursor(0, 1);
      lcd.print("ARDUINO IS BEST");
}

// 定义当前蓝牙状态
char bluetoothState;

// 定义计时器状态 默认计时器不启动
int timerStatus = 0; 

// 定义倒计时状态 默认倒计时不启动
int countDownStatus = 0;

// 倒计时初始数值
double countDownTimer = 0.0;

// 计时器初始数值
double timer = 0.0; 

// 存放计时器暂停时的数据
double timerRecord[3] = {0.0, 0.0, 0.0};

// 计时器模块
double timeKeepinger(double timer)
{
      delay(100);
      timer += 0.1;
      lcd.clear();
      lcd.print("TIMER ON");
      lcd.setCursor(0,1);
      lcd.print(timer);
      return timer;
}

// 倒计时模块
double countDown(double countDownTimer)
{
      if(countDownTimer >= 0.0){
            delay(100);
            countDownTimer -= 0.1;
            lcd.clear();
            lcd.print("COUNTDOWN ON");
            lcd.setCursor(0,1);
            lcd.print(countDownTimer);
      }
      return countDownTimer;
}

// 设定倒计时
double setCountDown(double countDownTimer, double perSec){
      countDownTimer += perSec;
      lcd.clear();
      lcd.print("SET COUNTDOWN:");
      lcd.setCursor(0, 1);
      lcd.print(countDownTimer);
      return countDownTimer;
}


void loop()
{
      if (mySerial.available() > 0)
      {     
            // 将蓝牙指令存入临时变量
            bluetoothState = mySerial.read();
            Serial.println(bluetoothState);

            // 当指令为A的时候，开启计时器
            if (bluetoothState == 'A')
            {     
                  countDownStatus = 0;
                  timerStatus = 1;
                  // 启动计时器
                  mySerial.println("Timer ON");
            }else if (bluetoothState == 'B') // 当指令为B的时候，停止计时器，并存储到数组里面
            {     
                  countDownStatus = 0;
                  timerStatus = 0;
                  // 停止计时器
                  mySerial.println("Timer OFF");
                  // mySerial.println(timer);
                  for(int i = 0; i < 3;i++){
                        if(timerRecord[i] == 0.0){
                              timerRecord[i] = timer;
                              // Serial.println(i);
                              mySerial.println(timerRecord[i]);
                              break;
                        }
                  }
            }else if (bluetoothState == 'C') // 当指令为C的时候，停止计时器，显示第一组结果
            {
                  countDownStatus = 0;
                  timerStatus = 0;
                  mySerial.println("Timer Record One");
                  delay(10);
                  lcd.clear();
                  lcd.print("First Record:");
                  lcd.setCursor(0,1);
                  lcd.print(timerRecord[0]);
            }else if (bluetoothState == 'D') // 当指令为D的时候，停止计时器，显示第二组结果
            {
                  countDownStatus = 0;
                  timerStatus = 0;
                  mySerial.println("Timer Record Two");
                  delay(10);
                  lcd.clear();
                  lcd.print("Second Record:");
                  lcd.setCursor(0,1);
                  lcd.print(timerRecord[1]);
            }else if (bluetoothState == 'E') // 当指令为E的时候，停止计时器，显示第三组结果
            {
                  countDownStatus = 0;
                  timerStatus = 0;
                  mySerial.println("Timer Record Three");
                  delay(10);
                  lcd.clear();
                  lcd.print("Third Record:");
                  lcd.setCursor(0,1);
                  lcd.print(timerRecord[2]);
            }else if(bluetoothState == 'F') // 当指令为F的时候，设定倒计时加1秒
            {     
                  timerStatus = 0; 
                  countDownTimer = setCountDown(countDownTimer, 1.0);
                  mySerial.println("SET CountDownTimer");
            }else if(bluetoothState == 'G') // 当指令为F的时候，设定倒计时加10秒
            {
                  timerStatus = 0;
                  countDownTimer = setCountDown(countDownTimer, 10.0);
                  mySerial.println("SET CountDownTimer");
            }else if(bluetoothState == 'H') // 开始倒计时
            {     
                  timerStatus = 0;
                  countDownStatus = 1;
                  // 启动倒计时
                  mySerial.println("CountDownTimer ON");
            }else if(bluetoothState == 'R') // 重置
            {
                  void(* resetFunc) (void) = 0;  // declare reset fuction at address 0
                  resetFunc(); // call reset
            }else if(bluetoothState == 'I') // 显示作者信息
            {
                  timerStatus = 0;
                  countDownStatus = 0;
                  lcd.clear();
                  lcd.print("AUTHOR:");
                  lcd.setCursor(0,1);
                  lcd.print("  YANG YINGTAO");
            }
      }
      // 检测计时器状态是否为1
      if (timerStatus == 1)
      {
            timer = timeKeepinger(timer);
      }
      // 检测倒计时状态是否为1
      if (countDownStatus == 1)
      {
            countDownTimer = countDown(countDownTimer);
      }
}
