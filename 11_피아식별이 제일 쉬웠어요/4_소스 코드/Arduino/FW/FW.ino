#include <SoftwareSerial.h>
#define LED_PIN 13  //LED output
#define BUZZER_PIN 11 //Buzzer output
#define PIR_PIN 8   //PIR input
#define SIGNAL_SEND '1' //BT Signal out
#define SIGNAL_RECV '1' //BT Signal in
#define SIGNAL_TREAT '2' //BT Signal threat

SoftwareSerial BTSerial(3, 2); //BT tx,rx

int pirState = LOW; // PIR default
int detectFlag = LOW; // Signal 입력값
char btFlag = '0';
unsigned long startTime, endTime; //Time out설정을 위한 값
 
void setup() 
{
    BTSerial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(PIR_PIN, INPUT);

    Serial.begin(9600);
}
 
void loop()
{
    detectFlag = digitalRead(PIR_PIN); // 센서값 읽기
    if (detectFlag == HIGH)  //적외선 감지
    { 
        digitalWrite(LED_PIN, HIGH); // LED ON
        if (pirState == LOW) 
        {
            // 시리얼모니터에 메시지 출력
            Serial.println("Motion detected");
            pirState = HIGH;
            BTSerial.write(SIGNAL_SEND);
            Serial.println("BT sended");
            startTime = micros();
            while(true)
            {
              endTime = micros();
              btFlag = BTSerial.read();
              if(btFlag == SIGNAL_RECV) //처리 완료 신호 수신
              {
                Serial.println("BT received");
                break;
              }
              if(btFlag == SIGNAL_TREAT) //위협으로 판단 신호 수신
              {
                Serial.println("BT threat received");
                tone(BUZZER_PIN, 500, 1000);
                break;
              }
              if(endTime - startTime > 30000000) //30초후 Time out
              {
                Serial.println("Time out");
                break;
              }
            }
        }
    } 
    else 
    {
        digitalWrite(LED_PIN, LOW); // LED OFF
        if (pirState == HIGH)
        {        
            // 시리얼모니터에 메시지 출력            
            Serial.println("Motion ended");
            pirState = LOW;
        }
    }
}
