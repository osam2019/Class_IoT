#include <SoftwareSerial.h>
#define BT_TXD 3
#define BT_RXD 2
#define DigitalPin1 6 
#define DigitalPin2 7
#define FULL 50
#define HALF 25

SoftwareSerial bluetooth(BT_RXD, BT_TXD);

int touched1;
int touched2;
int cnt = 0;
int descending = 0;

void setup ()
{
  pinMode (DigitalPin1, INPUT);
  pinMode (DigitalPin2, INPUT);
  Serial.begin (9600);
  bluetooth.begin(9600);
}

void loop ()
{
  // 술을 따를 때에만 측정이 되게 한다.
  if (digitalRead (DigitalPin1)){
      touched1 = 1;
  }
  if (digitalRead(DigitalPin2)){
      touched2 = 1;
  }
   
  //터치 민감도를 최대로 키운다.
  if(cnt%1000==0){
    if(!(touched1||touched2)){
      Serial.println("Untouched");
      descending = 0;
    }
    
    else{
      if(touched2 || (touched2&&touched1)){
        if(descending==0){
          Serial.println ("Full detected");
          bluetooth.println(FULL);
        }
        Serial.println ("Touch2");
        touched1 = 0;
        touched2 = 0;
      }
      else if(touched1){
        if(descending==0){
          Serial.println ("Half detected");
          bluetooth.println(HALF);
        }
        Serial.println("Touch1");
        touched1 = 0;
      }
      descending = 1;
    }
  }
  cnt++;
  delay(1);

}
