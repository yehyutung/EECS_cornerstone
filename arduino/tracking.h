/***************************************************************************/
// File        [track.h]
// Author     [Erik Kuo]
// Synopsis   [Code used for tracking]
// Functions  [MotorWriting, MotorInverter, tracking]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#include <SoftwareSerial.h>
#include <Wire.h>

/*if you have no idea how to start*/
/*check out what you have learned from week 1 & 6*/
/*feel free to add your own function for convenience*/

/*===========================import variable===========================*/
int extern Mp;
void extern encounterNode();
int extern L1, L2, M, R1, R2;
void extern Detect();
bool extern state;
bool extern isBack;
/*===========================import variable===========================*/

void MotorWriting(double vR, double vL){
  double vValue_R;
  double vValue_L;
  
  if (vR>0 && vL>0){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }  
  else if (vR<0 && vL<0){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
  else if (vR>0 && vL<0){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else if (vR<0 && vL>0){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW); 
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
  
  vValue_R = abs(vR);
  vValue_L = abs(vL);
  
  analogWrite(MT_R, vValue_R);
  analogWrite(MT_L, vValue_L);  
}

// P/PID control Tracking

double _lastError = 0;
double totalError = 0;

int t = 0;

double powerCorrecting(int L1, int L2, int M, int R1, int R2){
    double errorR1 = 1.6, errorR2 = 1.4, errorM = 0, errorL1 = 1.6, errorL2 = 1.4;
    double _Kd = 70;
    double _Kp = 70;
    double errorRight = (errorR1*R1 + errorR2*R2 + errorM*M)/(L1 + L2 + M + R1 + R2);
    double errorLeft = (errorL1*L1 + errorL2*L2 + errorM*M)/(L1 + L2 + M + R1 + R2);
    double error = errorRight - errorLeft;
    double dError = error - _lastError;
    _lastError = error;
    totalError += error;
    return -_Kp*error - _Kd*dError;
}

void tracking(){
  
  if (L1 == 1 && L2 == 1 && M == 1 && R1 == 1 && R2 == 1){
    if(rfid()!= 0){
      delay(50);
      encounterNode();
    }else{
      if(!isBack) MotorWriting(130, 130);
      delay(30); //50
      Detect();
      if (L1 == 1 && L2 == 1 && M == 1 && R1 == 1 && R2 == 1){
      encounterNode();
      while((L1 + L2 + M + R1 + R2) > 3){
        delay(10);
        Detect();
      }
      BT.write("F");
      }
    }

  }else{
    if(L1 == 0 && L2 == 0 && M == 0 && R1 == 0 && R2 == 0){
      while(L1 == 0 && L2 == 0 && M == 0 && R1 == 0 && R2 == 0){
        if(isBack){
          MotorWriting(100, 100);
        }else{
          MotorWriting(-100, -100);
        }
        Detect();
      }
    }else{
        double error = powerCorrecting(L1, L2, M, R1, R2);
        if (isBack){
          error = -error;
        }
        MotorWriting(Mp - error, Mp + error);   
    }
  }
}
