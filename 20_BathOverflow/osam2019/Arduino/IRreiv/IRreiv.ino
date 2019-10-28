#include <IRremote.h>
#define dw digitalWrite
#define dr digitalRead
#define aw analogWrite
#define ar analogRead
#include <Servo.h>
Servo servo1,servo2;
#define recv 7

IRrecv irrecv(recv);
decode_results res;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(recv,INPUT);
  irrecv.enableIRIn();
  servo1.attach(11);
  servo2.attach(10);
  servo1.write(45);
  servo2.write(40);
}
int o1=9,o2=9;
void loop() {
  // put your main code here, to run repeatedly:

  if(irrecv.decode(&res)){
    if (res.value <=0xf && res.value>=0x0){Serial.println(res.value);
Serial.print(o1);        Serial.println(res.value,BIN);
#define D 150
        if(o1!=(res.value&0b0011)){
          o1=res.value&0b0011;Serial.print("a"); 
          switch(res.value&0b0011){ //냉수
            case 0b00: 
              for(int i=1;i<=7;i++){
                servo1.write(130-(130-45)*i/7); delay(D); 
              }
              break;
            case 0b01: 
              for(int i=1;i<=7;i++){
                servo1.write(45+(130-45)*i/7); delay(D); 
              }
              break;
            //case 0b10: servo1.write(80); break;
            //case 0b11: servo1.write(40); break;
          }
        }
        if(o2!=((res.value&0b1100)>>2)){
          o2=(res.value&0b1100)>>2;
          switch((res.value&0b1100)>>2){  //온수
            case 0b00: 
              for(int i=1;i<=7;i++){
                servo2.write(130-(130-45)*i/7); delay(D); 
              }
              break;
            case 0b01: 
              for(int i=1;i<=7;i++){
                servo2.write(45+(130-45)*i/7); delay(D); 
              }
            //case 0b10: servo2.write(80); break;
            //case 0b11: servo2.write(40); break;
          }
        }
        
        
    }
    
    irrecv.resume();
  }
}
