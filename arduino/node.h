/***************************************************************************/
// File        [node.h]
// Author     [Erik Kuo, Joshua Lin]
// Synopsis   [Code for managing car movement when encounter a node]
// Functions  [/* add on yout own! */]
// Modify     [2020/03/027 Erik Kuo]
/***************************************************************************/

#include <SoftwareSerial.h>
#include <Wire.h>

/*===========================import variable===========================*/
int extern L1, L2, M, R2, R1;
int extern Mp;
/*===========================import variable===========================*/

int delay_time = 400; 
  // 呼叫左轉副程式 参數為 delay_time 毫秒
void turnLeft() { 
  MotorWriting(120, -120); //設定小S左轉
  delay(delay_time); //執行700 毫秒
  while((M + R2 + L2) < 1){
    delay(50);
    Detect();
  }
  BT.write("e");
}

// 呼叫右轉副程式 参數為 delay_time 毫秒

void uTurn(){
  MotorWriting(-120, 120);
  delay(700);
  Detect();
  while((R2 + L2 + M) < 1){
    //delay(50);
    Detect();
  }
  BT.write("e");
}
void turnRight() { 
  MotorWriting(-120, 120); //設定小S右轉
  delay(delay_time);
  Detect();
  while(( M + R2 + L2) < 1){
    delay(50);
    Detect();
  }
  BT.write("e");
  //delay(delay_time); //執行700 毫秒
}

void encounterNode(){
  BT.write("N");
  /*
  if(L1 =2= 0 && L2 == 0 && M == 0 && R1 == 0 && R2 == 0){
    BT.write("T");
    while(L2 == 0 && M == 0 && R2 == 0){
      Detect();
      MotorWriting(-130, -130);
    }  
    delay(50);
    rfid();
  }
  */
  state = false;
  nodeCount++;
}
