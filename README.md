# Network Character LCD
Make your own network connected standalone LCD module with the help of character LCD and ESP8266 which can then be used in other electronics projects or from anywhere you can send an HTTP request.

## Prerequisites
- ESP8266 Development board
- An I2C Character LCD
- Breadboard and few jumper wires
- Wi-Fi Router for network connectivity
- Visual Studio Code with Platform IO extension

## Hardware Connections
Connect the pin mentioned in first column with second column for that row with help of jumper wires and breadboard.
| ESP Developement board  | I2C LCD |
| ------------- | ------------- |
| VIN  | VCC  |
| GND  | GND  |
| D1  | SCL  |
| D2  | SDA  |

## Getting Started
Open the project in Visual Studio Code using Platform IO home. Enter your WiFi username and password in `main.cpp` file. Build the project and it should download all the required dependencies. Connect the ESP8266 Development board and upload the code to it. The LCD should display the IP address when network is successfully connected.

### Display text on LCD
Send an HTTP `POST` request to `http://{ip_address_of_esp}/display` with content type of `application/json`. 

```
{
	"data": [
		{"text": "Line 1", "align":1},
		{"text": "This should appear on Line 2 of character lcd."}
	],
	"scroll":1
}
```

Check out [this blog post](https://dilpreet.dev/blog/network-character-lcd) for more details.

## Logs
Open serial monitor from bottom Platform IO bar in VS Code to see logs.

## Dependencies
- [Arduino JSON](https://github.com/bblanchon/ArduinoJson)
- [Liquid Crystal I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
