# Class_IoT

## Team Building
https://forms.gle/PVNjLcwVYaB1WpWj8
- 팀원 수 : 제한 없음 (개인 가능)

## 자료 취합
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
