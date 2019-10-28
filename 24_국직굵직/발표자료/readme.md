IoT_Guggicgulgic
===================

# 1. 프로젝트 이름
------------------
## 상병 자비스 (Corporal Jarvis)

# 2. 프로젝트 개요
--------------------
## 2.1 프로젝트 Overview

#### 실제 군 내에서 전화를 교환해주거나 전화를 받는 보직을 임하게 되면, 업무와 전혀 관계없는 일과 관련된 전화가 오는 경우가 매우 흔합니다.
저희 프로젝트 **상병 자비스**는 **Google Speech Recognition** 음성인식모듈과 **Chatbot**에 의한 답변학습을 활용하여, 위와 같은 발신자의
음성을 인식해, 그에 대한 답변을 전화기 하드웨어의 화면에 디스플레이해주는 AI기반 IoT 군 전화기입니다.

# 3. 상병 자비스 블록다이어그램
-------------------------
####
링크 올려드리겠습니다.
[Block Diagram of Jarvis](https://www.draw.io/?state=%7B%22ids%22:%5B%221iwCvxclkZYj36zZODpW1UAqr17ZMdT-A%22%5D,%22action%22:%22open%22,%22userId%22:%22101443275628626820839%22%7D#G1iwCvxclkZYj36zZODpW1UAqr17ZMdT-A)
사진도 올려드리겠습니다.
![Block_Diagram](https://user-images.githubusercontent.com/54850462/67497036-53b07780-f6b8-11e9-8f18-d50d27cbe45f.jpg)

# 4. 상병 자비스 매뉴얼
----------------------
####
사용자의 관점에서 서술하겠습니다.
1) 수화기를 들면 LCD의 Backlight가 켜지고 LCD 화면에는 **Corporal Jarvis**라는 서비스 이름이 출력됩니다.

2) RFID 카드를 RFID 센서가 부착되어 있는 전화기의 하드웨어에 대면, 삑 소리와 함께 사용자의 음성이 웹으로 보내지고
   RFID 카드를 때면 삐빅 소리와 함께 음성 전송이 종료됩니다. 음성 전송 중에는 LCD에 **로딩 중**을 의미하는 애니메이션이
   보여집니다.
   
3) RFID를 태그할 동안 입력된 음성에 대한 답변이 LCD에 디스플레이되며, 답변의 사이즈가 16byte를 넘을 경우 Autoscroll을 지원해
   문구가 LCD에 슬라이드됩니다.
   
4) 입력된 질문과 출력된 답변은 사용자의 앱 UI에 메신저 형식으로 표시되고, 서버의 repository에 자동저장됩니다.

5) RFID가 센서에 입력되면 휴대폰의 카메라가 웹으로 사용자의 캡쳐사진을 보내주며, 그 이후 카메라의 촬영화면이 웹에 스트리밍됩니다.

# 5. 상병 자비스 동작코드 및 구조
-------------------------------
## 5.1 사운드센서의 음성을 아두이노 AVR 보드로

####
기능 : 라즈베리파이는 오디오의 아날로그 입력을 지원하지 않기 때문에, 사운드센서로 입력된 사용자의 음성 raw data는 반드시 아두이노를 
거쳐야 한다. 하지만 아두이노의 sampling rate와 처리속도의 저조함, ADC 비트의 비호환성을 해결하기 위해 아두이노를 AVR 보드로 구웠다.
아래 코드는 사운드 센서에서 입력된 사용자의 음성을 audio-streaming해 라즈베리파이로 보내주는 c기반의 코드이다.

```c
#define F_CPU 16000000UL
#define USART_BAUDRATE 230400
#define UBRR_VALUE ((F_CPU/4/USART_BAUDRATE)-1)/2

// Set the baud rate divider to 8 to double the
// transfer rate
UCSR0A |= (1<<U2X0);
// Set baud rate based on UBBR_VALUE
UBRR0H = (uint8_t)(UBRR_VALUE>>8);
UBRR0L = (uint8_t)UBRR_VALUE;
// Set frame format to 8 data bits, no parity, 1 stop bit
UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
// Enable transmission and reception
UCSR0B |= (1<<RXEN0)|(1<<TXEN0);

// Initialize debug ports
void InitPort(void)
{
 // Set PD6 and PD2 as output
 DDRD |= (1<<PD2)|(1<<PD6);
}

// Initialize timer0
void InitTimer0(void)
{
 // Set Initial Timer value
 TCNT0=0;
 // Place TOP timer value to Output compare register
 OCR0A=99;
 // Set CTC mode
 // and make toggle PD6/OC0A pin on compare match
 TCCR0A |=(1<<COM0A0)|(1<<WGM01);
}

// Start timer0 with prescaler 8
void StartTimer0(void)
{
 // Set prescaler 8 and start timer
 TCCR0B |=(1<<CS01);
}
void InitADC()
{
 // Select Vref=AVcc
 // and set left adjust result
 ADMUX |= (1<<REFS0)|(1<<ADLAR);
 // Set prescaler to 32
 // Enable auto-triggering
 // Enable ADC interrupt
 // and enable ADC
 ADCSRA |= (1<<ADPS2)|(1<<ADPS0)|(1<<ADATE)|(1<<ADIE)|(1<<ADEN);
 // Set ADC trigger source - Timer0 compare match A
 ADCSRB |= (1<<ADTS1)|(1<<ADTS0);
}
void SetADCChannel(uint8_t ADCchannel)
{
 // Select ADC channel with safety mask
 ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
}
void StartADC(void)
{
 ADCSRA |= (1<<ADSC);
}
ISR(ADC_vect)
{
 // Clear timer compare match flag
 TIFR0=(1<<OCF0A);
 // Toggle pin PD2 to track the end of ADC conversion
 PIND = (1<<PD2);
 // Wait while previous byte is completed
 while(!(UCSR0A&(1<<UDRE0))){};
 // Transmit data
 UDR0 = ADCH;
}
int main(void)
{
 // Initialize USART0
 USART0Init();
 // Initialize ports
 InitPort();
 // Initialize ADC
 InitADC();
 // Select ADC channel
 SetADCChannel(0);
 // Initialize timer0
 InitTimer0();
 // Start timer0
 StartTimer0();
 // Start conversion
 StartADC();
 // Enable global interrupts
 sei();

 while(1)
 { 
 }
}
```
## 5.2 CDS/ RFID/ LCD
####
조도센서가 수화기의 on/off 상태를 체크하고, 조도센서가 빛을 받으면 LCD의 backlight가 켜진다. RFID를 센서에 찍는 순간, RFID의 정보를 번호로
mapping하여 라즈베리파이에 보내줍니다. RFID가 센서에서 탈착되는 순간 0을 보내줍니다. 0이 아닌 정수가 라즈베리파이로 보내진다면, audio-recording을
진행하며 0이 보내진다면 audio-recording을 멈춥니다. 보내진 RFID 정보는 이후 서버에서 재처리될 것입니다.
라즈베리파이로부터 수신한 답변 텍스트는 LCD에 출력됩니다.

코드:
```C
#include<LiquidCrystal_I2C_Hangul.h>
#include<Wire.h>

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); //j Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4][4] = {{0xA8, 0x88, 0x4A, 0x27},
                       {0x98, 0xBF, 0xA8, 0x27},
                       {0xDC, 0x93, 0xBF, 0x49},
                       {0x60, 0x2D, 0xC2, 0x49}};

byte preId[4];

int line = 0;

char box = 0xff;

int rfid_num = 0;
byte answer_str[50];
int answer_str_len=0;
bool lcd_answer_flag=false;
unsigned long time;
int time_count=                      -15;
LiquidCrystal_I2C_Hangul lcd(0x3f,16,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init();
  lcd.noBacklight();
  lcd.setCursor(2,0);
  lcd.print("Corporal");
  lcd.setCursor(8,1);
  lcd.print("Jarvis");
  
  SPI.begin(); // Init SPI bus

  pinMode(8, OUTPUT);
  
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  pinMode(A0, INPUT_PULLUP);
  

 // Serial.println(F("This code scan the MIFARE Classsic NUID."));
  //Serial.print(F("Using the following key:"));
  //printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = analogRead(A0);
  int interval=300;
  if(lcd_answer_flag==true && millis()-time>interval){
    //32
    lcd.setCursor(0,1);
    for(int i=0;i<16;i++){
      if(i+time_count>=answer_str_len || i+time_count<0) lcd.write(32);
      else lcd.write(answer_str[i+time_count]);
    }
    time+=interval;
    time_count++;
    if(time_count==answer_str_len) time_count=-15;
  }
  if(a<600) {
    lcd.backlight();
    rfid_num = rfid_control();
    lcd_control(rfid_num);
    }
  else {
    lcd_answer_flag=false;
    lcd.noBacklight();
    lcd.setCursor(0,0);
    lcd.print("  Corporal      ");
    lcd.setCursor(0,1);
    lcd.print("        Jarvis  ");
    }
}

void lcd_control(int rfid_num) {
  String str;
  if (rfid_num == 1) {
    str = "Jung Min";
    }
  else if (rfid_num == 2) {
    str = "Seong Jin";
    }
  else if (rfid_num == 3) {
    str = "Won Seok";
    }
  else {
    str = "Kyun Chul";
    }
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    lcd.setCursor(0,0);
    lcd.print("Name : " + str);
    lcd.setCursor(0,1);
    answer_str_len=0;
    time_count=-15;
    while (Serial.available() > 0) {
      // display each character to the LCD
      answer_str[answer_str_len++]=Serial.read();
    }
    lcd_answer_flag=true;
    time=millis();
  }
  }

int rfid_control() {
   // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //Serial.print(F("PICC type: "));
  //MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  //if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
  //  piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
  //  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
  //  Serial.println(F("Your tag is not of type MIFARE Classic."));
  //  return;
  //}

  int i, j;
  int count = 0;
  int box_count = 2;
  int box_reverse = 0;
  int box_cursor = 7;
  int rfid_id = 0;

  //tone(8, 392, 4);
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);

  for(i = 0; i < 4; i++){
    count = 0;
    for(j = 0; j < 4; j++){
      if(rfid.uid.uidByte[j] == nuidPICC[i][j])count++;
    }
    if(count == 4){
      rfid_id = (i+1);
      Serial.write(rfid_id);
      for(j = 0; j < 4; j++){
        preId[j] = rfid.uid.uidByte[j];
      }
    }
    
  }

//  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
//    rfid.uid.uidByte[1] != nuidPICC[1] || 
//    rfid.uid.uidByte[2] != nuidPICC[2] || 
//    rfid.uid.uidByte[3] != nuidPICC[3] ) {
//    Serial.println(F("A new card has been detected."));
//
//    // Store NUID into nuidPICC array
//    for (byte i = 0; i < 4; i++) {
//      nuidPICC[i] = rfid.uid.uidByte[i];
//    }
//   
//    Serial.println(F("The NUID tag is:"));
//    Serial.print(F("In hex: "));
//    printHex(rfid.uid.uidByte, rfid.uid.size);
//    Serial.println();
//    Serial.print(F("In dec: "));
//    printDec(rfid.uid.uidByte, rfid.uid.size);
//    Serial.println();
//  }
//  else Serial.println(F("Card read previously."));

//  while(true){
//    rfid.PICC_IsNewCardPresent();
//    for(i = 0; i < 4; i++){
//      if(rfid.uid.uidByte[i] == preId[i]){
//        count++;
//      }
//    }
//    printHex(rfid.uid.uidByte, 4);
//    printHex(preId, 4);
//    Serial.println();
//    if(count < 4) break;
//    delay(10);
//  }
  lcd_answer_flag=false;
  lcd.clear();
  while(rfid.PICC_ReadCardSerial() || rfid.PICC_IsNewCardPresent()){
    if(box_reverse == 0) {
      lcd.setCursor(box_cursor,0);
      for(i = 0; i < box_count; i++) {
        lcd.print(box);
        }
      lcd.setCursor(box_cursor,1);
      for(i = 0; i < box_count; i++) {
        lcd.print(box);
        }
      box_count += 2;
      box_cursor--;
      }
    if(box_count == 18) {
        box_cursor = 0;
        box_reverse = 1;
        box_count = 16;
        }
     
    if(box_reverse == 1) {
      lcd.setCursor(box_cursor,0);
      for(i = 0; i < box_count; i++) {
        lcd.print(box);
        }
      lcd.setCursor(box_cursor,1);
      for(i = 0; i < box_count; i++) {
        lcd.print(box);
        }
      box_count -= 2;
      box_cursor++;
      }
      if(box_count == -2) {
        box_cursor = 7;
        box_reverse = 0;
        box_count = 2;
        }
    delay(64);
    lcd.clear();
  }

  lcd.clear();

  Serial.write(0);
  digitalWrite(8, HIGH);
  delay(50);
  digitalWrite(8, LOW);
  delay(50);
  digitalWrite(8, HIGH);
  delay(50);
  digitalWrite(8, LOW);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return rfid_id;
  }
  ```
## 5.3 Raspberry Pi

## 5.4 Web

## 5.5 App

# 6. 기대효과 및 발전사항

####
1) 학습되는 데이터의 양이 증가할수록 음성인식모듈의 인식능력이 올라갈 것이며, 이는 커뮤니티의 발전과 병행될 것이라고 전망한다.
2) 군대에서 독점적으로 쓰이는 용어들이 Google-Recognition-Data에 없는 것을 확인했으며, 이는 구글 음성 인식모듈의 정확성을 올려주는 
상호 협력적 관계를 형성될 것이다.
3) RFID나 카메라를 이용해 군에서의 필수 조건인 보안에 초점을 맞췄으며, RFID ID의 체계화를 통한 통신상의 보안강화도 이룰 수 있을 것이다.

