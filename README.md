# arduino-pagerduty
Check PagerDuty on your ESP8266 and SSD1306 powered display

## Instructions
I use an ESP8266 Chip with Wifi and an 0.96" OLED display with SPI to display open incidents on PagerDuty. 

## Needed Libraries
- WifiManager
- SSD1306 display library from squix78
- ArduinoJson

## Setup
Within the code you need to add PagerDutyDomain and PagerDutyToken with your PagerDuty settings. Compile it, deploy it, setup Wifi and be happy

## Example
![Example](https://github.com/bert2002/arduino-pagerduty/raw/master/images/display.png)


