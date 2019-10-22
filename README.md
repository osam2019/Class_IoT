# Class_IoT

## Team Building
https://forms.gle/PVNjLcwVYaB1WpWj8
- 팀원 수 : 제한 없음 (개인 가능)

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

## AVR ISP 사용법
    sudo avrdude -P /dev/ttyUSB0 -p atmega328p -c avrispv2
