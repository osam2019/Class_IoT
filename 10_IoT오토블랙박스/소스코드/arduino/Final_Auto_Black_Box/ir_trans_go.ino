// 최초 미확인 차량이 접근했을때 1차 IR 신호 송신
void ir_trans_go() {
  

    for (int i = 0; i < 30; i++)
      {
          irsend.sendNEC(0xff6897, 32); // 송신 코드, 송신 코드의 비트 수 // 블랙박스 전원 차단 신호 송출

          delay(20);
       
      } 
    Serial.println("블랙박스 차단.");
    irrecv.enableIRIn(); //수신 센서 다시 재기동
    
}
