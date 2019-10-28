void ultrasound() { 
  
//***************************************************************************************************//
delay(system_time); // 시스템 시간
//#1. 초음파 센서의 데이터를 ultrasonic_count 만큼 데이터를 감지 후 에 저장한다.
distance1_a = 0;
distance2_a = 0;
/*** 초음파1 감지 ***/ 
 for (int i=1; i <= ultrasonic_count; i++){  
 digitalWrite(trigPin1, LOW); 
 delayMicroseconds(2);  
 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10);  
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH);
 distance1 = duration1/58.2; // 측정된 시간을 cm로 환산
 distance1_a = distance1_a + distance1;
 delay(ultrasonic_time);
}
/*** 초음파2 감지 ***/  
for (int i=1; i <= ultrasonic_count; i++){ 
 digitalWrite(trigPin2, LOW); 
 delayMicroseconds(2);  
 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10);  
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH);
 distance2 = duration2/58.2; // 측정된 시간을 cm로 환산 
 distance2_a = distance2_a + distance2;
 delay(ultrasonic_time);
}
//***************************************************************************************************//
//***************************************************************************************************//
//#2. 감지한 데이터의 평균 값을 산출 한다. 
distance1= long(distance1_a/ultrasonic_count);
distance2= long(distance2_a/ultrasonic_count);
//***************************************************************************************************//
//***************************************************************************************************//
//#3. 초음파 측정 거리를 제한한다. - maxiumrange만큼만 측정 할 수 있도록 제한을 둔다. 
 if (distance1 >= maximumRange){  distance1 = maximumRange; }  
 if (distance2 >= maximumRange){  distance2 = maximumRange; } 
//***************************************************************************************************//
distance = (distance1+distance2) / 2;


 /*** 동작여부 확인 - 컴퓨터 시리얼모니터 ***/ 
//  Serial.println(distance);



}
