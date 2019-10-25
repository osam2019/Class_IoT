# Class_IoT

#### Pull Request / Issue 환영합니다.

## Team Building
https://forms.gle/PVNjLcwVYaB1WpWj8
- 팀원 수 : 제한 없음 (개인 가능)

## Team

|No|팀명|팀원|
|--|---------|-----------|
|1|하늘을보아라|김정현|
|2|FLEX|전유택, 정해준|
|3|일상병|차기욱, 조도운, 주상현|
|4|SYR|신정우, 전인호, 서준표|
|5|AWS|방준식, 김민욱|
|6|스마트텀블러|황선우, 신대현|
|7|비염시러|김정훈, 곽진성|
|8|빛으로강타해요!|김한준, 임동건, 원지운|
|9|W.A.Y(Where Are You)|정재엽, 한정훈, 허재욱, 홍동원|
|10|IOT_오토블랙박스|이경덕, 이은표, 한성민|
|11|피아식별이 제일 쉬웠어요|주일규, 박현진|
|12|콩콩|곽재원, 김남현|
|13|DreamCar|김동현, 심규원|
|14|집에가자|김경현, 조진환, 정준원|
|15|받고더블로가!|정조형, 주동현, 최호수, 임원균|
|16|배달의 민족|임성민, 양지민|
|17|아직 한잔 남았다|황세진, 오승민, 정승규|
|18|만들어듀오|최영석, 이상목|
|19|10% Human|권용훈|
|20|BathOverflow|백창수, 정병준, 김동훈, 김재현|
|21|Vendi|이상운|
|22|돈을 아껴써야조|박성현, 구경회|
|23|태풍|김만철, 정현석|
|24|국직굵직|이건철, 조정민, 정원석, 최성진|
|25|당신의 허리는 안녕하십니까? |이기정|

## 자료 취합 ( ~ 2019. 10.25(금) AM 08:00 KST 까지 업로드 받습니다. )
https://drive.google.com/drive/folders/1ft68sSdJ1A6nfnwM7XJNP1Wtv3ka_iad?usp=sharing

### 필수 (취합하여 위 Git에 일괄 업로드 예정입니다.)
1. 발표자료
2. 시연 영상
3. 개발과정(영상,사진 등)
4. 코드


## How to connect USB ISP with VirtualBox

Ref Link :  https://bytefreaks.net/windows/virtualbox-failed-to-attach-the-usb-device-to-the-virtual-machine

1. regedit 실행
* WindowKey + R 
* regedit 입력

2. 레지스트리 위치 확인 
* HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Class\{36FC9E60-C465-11CF-8056-444553540000}

3. UpperFilters 값 삭제
* 우클릭->삭제

4. Virtual Box USB 드라이버 재설치
* C:\Program Files\Oracle\VirtualBox\drivers\USB\filter 경로 이동
* VBoxUSBMon.inf (3KB, 설치정보라는 유형으로 분류) 우클릭 - 설치

5. 재부팅

## AVR ISP 사용법 (Check)
    sudo avrdude -P /dev/ttyUSB0 -p atmega328p -c avrispv2
### Arduino를 ISP를 이용하여 Firmware Upload시, ATmega328p인지, ATmega16u2인지 확인 필요!
#### ICSP라고 적혀있는 커넥터가 328P 입니다. 이쪽으로 연결하여 펌웨어를 업로드 하시면 됩니다. 
    
## AVR ISP Firmware Upload
    sudo avrdude -P /dev/ttyUSB0 -p atmega328p -c avrispv2 -U flash:w:target_firmware.hex

## Arduino Uno Pinout
Ref Link : https://www.circuito.io/blog/arduino-uno-pinout/
