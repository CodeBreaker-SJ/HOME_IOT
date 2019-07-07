# HOME_IOT

## Description
------------------------------------------------------------
각 IOT 장치는 초기 아이디를 임의 생성하여 서버와 통신한다.

각 IOT 장치는 서버에 접속하여 자동 생성한 아이디를 버리고 서버로 부터 부여 받은 아이디를 사용한다.

IOT 장치는 부여 받는 아이디를 ROM에 저장하여 재부팅시 이를 사용하여 서버에 접속한다.

모든 IOT 장치는 MQTT 프로토콜을 사용하여 통신한다.


## Data Format
------------------------------------------------------------
명령체계 (JSON)
{
 id: '',
 cmd: '',
 data: ''
} 

OPEN
CLOSE
ON
OFF
TEMP
HUM


## Structure
------------------------------------------------------------
- Central
JSP, MQTT Broker, InfluxDB, MariaDB
미러디스플레이
음성인식

- Maintain & Measurement
IOT_소리, IOT_온습도, IOT_조도, IOT_화재감지, IOT 도어센서, IOT_모션, IOT_도어락, IOT_전원제어, IOT_버저, IOT_레이저거리 센서, IOT_RC카

- Security
Motion Detection Camera
CCTV (Movement & Streaming)



------------------------------------------------------------

- Command Linker
장치 커맨드 타입 값 
스케줄?

