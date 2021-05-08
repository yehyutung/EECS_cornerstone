/***************************************************************************/
// File        [bluetooth.h]
// Author     [Erik Kuo]
// Synopsis   [Code for bluetooth communication]
// Functions  [ask_BT, send_msg, send_byte]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 2*/
int extern nodeCount;
#include<SoftwareSerial.h>
enum BT_CMD {
  NOTHING,
  // TODO: add your own command type here
  W, //Move Forward
  A, //Turn Left
  S, //Move Backward
  D, //Turn Right
  H, //Halt
  U, //u-turn
};

BT_CMD ask_BT(){
    BT_CMD message=NOTHING;
    int cmd;
    if(BT.available()){
      cmd = BT.read(); //get cmd from SoftwareSerial object: BT
      // TO-DO: link bluetooth message to your own command type
      switch (cmd){
        case 49:
        message = W;
        break;
        case 50:
        message = U;
        break;
        case 51:
        message = D;
        break;
        case 52:
        message = A;
        break;
        case 53:
        message = H;
        break;
        case 65:
        message = H;
        break;
      }
      #ifdef DEBUG
      Serial.print("cmd : ");
      Serial.println(message);
      #endif
    }
    return message;
}// ask_BT

BT_CMD bloodDie(){
  // FOR CHECKPOINT
  switch(nodeCount){
    case 4:
    return A;
    break;
    case 5:
    return D;
    break;
    case 6:
    return A;
    break;
    case 7:
    return D;
    break;
    case 8:
    return U;
    break;
    case 10:
    return H;
    break;
 
    default:
    return W;
    break;
  }
}
// send msg back through SoftwareSerial object: BT
// can use send_byte alternatively to send msg back
// (but need to convert to byte type)
void send_msg(const char& msg)
{
     // TODO:
     BT.write(msg);
}// send_msg

// send UID back through SoftwareSerial object: BT
void send_byte(byte *id, byte& idSize) {
  BT.write("T");
  for (byte i = 0; i < idSize; i++) {  // Send UID consequently.
    BT.write(id[i]);
  }
  #ifdef DEBUG
  Serial.print("Sent id: ");
  for (byte i = 0; i < idSize; i++) {  // Show UID consequently.
    Serial.print(id[i], HEX);
  }
  Serial.println();
  #endif
}// send_byte