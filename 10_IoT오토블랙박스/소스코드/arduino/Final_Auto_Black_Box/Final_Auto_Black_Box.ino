/*
 제목    : 오토블랙박스에 제어 IR 신호를 주는 디바이스
 내용   :  위병소에서 출입용, 퇴영용 두개가 설치
 */


#include <IRremote.h>
int RECV_PIN = A0; //define input pin on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results;
int power = 1;

IRsend irsend; // ir 송신을 위해 D3 pin 고정으로 할당.

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 9); //블루투스의 Tx, Rx핀을 2번 9번핀으로 설정


int reset = 12; // 블랙박스 차단 장비가 설치되어 있지 않는 상황을 해결하기 위한 경고음 해제 스위치를 선언합니다.
int led = 8; // IR 송신모듈이 없는 상황이라 송신되는 상황을 표현하기 위해 led로 선언합니다.
int sound = A1; // 오토블랙박스가 설치되어 있지 않는 차량이 출입할때 경고음을 발생시킬 부저를 선언합니다.
boolean trans_push = false; // 최초 접근시 
boolean receive_get = false; // led 상태
unsigned long master_key = 0; // 사용자에게 받은 IR 신호 저장소
int key = 4294967295;



//******************************************************초음파 관련 소스*************************************//

/*************** 고정 변수 선언 ***************/
/*초음파 스펙 : resolution : 0.3 cm, ranging distance : 2cm – 500 cm */
// 초음파#1
#define trigPin1 6 // Trigger Pin
#define echoPin1 7 // Echo Pin
// 초음파#2
#define trigPin2 4 // Trigger Pin
#define echoPin2 5 // Echo Pin 
// 초음파 최대-최소거리 설정 
#define maximumRange   100   // 최대 거리
#define minimumRange   10    // 최소 거리


// 변수선언
const int system_time = 50;                      // 전체 시스템 시간 
const int ultrasonic_time = 50;                  // 초음파 측정 시간 
long duration1,duration2, distance1, distance2;  // 펄스 시간, 거리 측정용 변수
long distance1_a, distance2_a;                   // 구동연산 변수
int ultrasonic_count = 3;                        // 초음파 Count만큼 감지 후 평균 값에 의해 동작 함
const long distanse_set = 27;                    // 초음파 사람 감지 세팅 값 - 해당 거리 일 경우에 사람 혹은 물체로 인지 함
long distance;

//******************************************************초음파 관련 소스*************************************//



void setup() {
    irrecv.enableIRIn(); // Start the receiver (적외선 통신)  
    Serial.begin(9600); // 초음파센서의 동작 상태를 확인하기 위하여 시리얼 통신을 설정합니다. (전송속도 9600bps)
      while (!Serial) {
    ; //시리얼통신이 연결되지 않았다면 코드 실행을 멈추고 무한 반복
  }
    mySerial.begin(9600); //블루투스와 아두이노의 통신속도를 9600으로 설정
    Serial.println("Hello World!");


    pinMode(led, OUTPUT); // LED핀 -> OUTPUT
    pinMode(sound, OUTPUT);  
    pinMode(reset, INPUT_PULLUP);


    pinMode(trigPin1, OUTPUT); 
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
  
    delay(system_time); 


}
 



void loop() {

  ultrasound();
  
  if (irrecv.decode(&results)) {  // 적외선 수신
            Serial.println(results.value); 
            master_key = results.value;
            Serial.write(results.value);
            irrecv.resume();
             
          }
          
   
  if (mySerial.available()) { //블루투스에서 넘어온 데이터가 있다면
    if (mySerial.read() == 49){
      power = 1;
      digitalWrite(sound,LOW);    
      digitalWrite(led, LOW);
      } //시리얼모니터에 데이터를 출력
    mySerial.flush();
  }
  
    digitalWrite(reset, HIGH);
  
  if(power) {
        if (distance >= 15) {
            Serial.println("차량이 접근하지 않았습니다.");    
        } else if (distance < 15&&master_key == 0&&distance >=7) {
            Serial.println("차량이 감지되었습니다.");
            ir_trans();
                        
        } else if (distance < 7&&master_key == 0) {
          alert();   
        }

        power = match_key();
  }
       
if (digitalRead(reset) == LOW) {
            alert_reset();  
            master_key = 0;
            power = 1;
        }
    delay(1000); // 0.2초 동안 대기합니다.
}
