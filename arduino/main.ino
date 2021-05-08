/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search_Mode, Hault_Mode, SetState]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG = true // debug flag

// for BlueTooth
#include<SoftwareSerial.h>
// for RFID
#include <SPI.h>
#include <MFRC522.h>

/*===========================define pin & create module object================================*/
// BlueTooth
SoftwareSerial BT(3,9);   // TX,RX on bluetooth module, 請按照自己車上的接線寫入腳位

#define MT_R 5 //ENB
#define MT_L 6 //ENA
#define IN1 2
#define IN2 4
#define IN3 7
#define IN4 8
#define TCRT_L1 A4
#define TCRT_L2 A3
#define TCRT_M A2
#define TCRT_R2 A1
#define TCRT_R1 A0
// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN      A5        // 讀卡機的重置腳位
#define SS_PIN       10       // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup()
{
   //bluetooth initialization
   BT.begin(9600);
   //Serial window
   Serial.begin(9600);
   //RFID initial
   SPI.begin();
   mfrc522.PCD_Init();
  //L298N pin
  pinMode(MT_L,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(MT_R,OUTPUT);
  pinMode(TCRT_L1,INPUT);
  pinMode(TCRT_L2,INPUT);
  pinMode(TCRT_M,INPUT);
  pinMode(TCRT_R2,INPUT);
  pinMode(TCRT_R1,INPUT);
#ifdef DEBUG
  Serial.println("Start!");
#endif

}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
#include "track.h"
#include "bluetooth.h"
#include "node.h"
/*=====Import header files=====*/

/*===========================initialize variables===========================*/
int L1 = 0, L2 = 0, M = 0, R2 = 0, R1 = 0; //紅外線模組的讀值(0->white,1->black)
int MpValue = 130;
int Mp = MpValue; //set your own value for motor power
bool state=true; //set state to false to halt the car, set state to true to activate the car
BT_CMD _cmd = NOTHING; //enum for bluetooth message, reference in bluetooth.h line 2
int nodeCount = 0;
bool isStart = false;
bool isBack = false;
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();// search graph
void SetState();// switch the state
void Detect();
/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/
void loop()
{ 
  if(isStart){
   Detect();
   if(!state) MotorWriting(0,0);
   else Search();
   SetState();
  }else{
    int reading = 0;
    if(BT.available())reading = BT.read();
    if(reading == 115){
      isStart = true;
    }
  }
  
}

void SetState()
{
  //_cmd = ask_BT();
  _cmd = bloodDie();
  if(!state){
    state = true;
    BT.write("K");
    switch(_cmd){
      case W:
      isBack = false;
      Mp = MpValue;
  //    MotorWriting(Mp, Mp);
 //     delay(250);
      break;
      
      case A:
      isBack = false;
      Mp = MpValue;
      BT.write("l");
      turnLeft();
      break;
      
      case S:
      isBack = true;
      BT.write("s");
      Mp = -MpValue;
      break;
      
      case D:
      isBack = false;
      Mp = MpValue;
      BT.write("r");
      turnRight();
      break;

      case H:
      //BT.write("h");
      state = false;
      break;
      
      case U:
      isBack = false;
      Mp = MpValue;
      BT.write("u");
      uTurn();
      break;
      
      default:
      isBack = false;
      Mp = MpValue;
      break;
    }
  }
}


void Detect(){
    L1 = digitalRead(TCRT_L1);
    L2 = digitalRead(TCRT_L2);
    M = digitalRead(TCRT_M);
    R1 = digitalRead(TCRT_R1);
    R2 = digitalRead(TCRT_R2);
}

void Search(){
  // TODO: let your car search graph(maze) according to bluetooth command from computer(python code)
  tracking();
}
/*===========================define function===========================*/