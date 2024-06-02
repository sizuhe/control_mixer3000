`Mixer3000` is a system capable of measuring weight and temperature, pumping and mixing water, and lifting up and down a container. All of these capabilities can be used in real time with an Arduino Nano (or UNO) and respective commands.



# Hardware
- Arduino Nano or UNO
- HX711
- A4988 (2 units)
- L293D
- DS18B20
- HC020K
- HC-05



# Components
| Arduino Nano (MCU) | HX711 (Load Cell) | A4988 (Step 1) | A4988 (Step 2) | L293D (Pump) | DS18B20 (Temperature) | HC020K (Encoder) | HC-05 (Bluetooth) |
|---|---|---|---|---|---|---|---|
| A0 | DOUT | - | - | - | - | - | - |
| A1 | CLK | - | - | - | - | - | - |
| D2 | - | STEP | - | - | - | - | - |
| D3 | - | - | STEP | - | - | - | - |
| D4 | - | - | DIR | - | - | - | - |
| D5 | - | DIR | - | - | - | - | - |
| D6 | - | ENA | - | - | - | - | - |
| D7 | - | - | - | - | - | DOUT | - |
| D8 | - | - | ENA | - | - | - | - |
| D9 | - | - | - | PIN 1 | - | - | - |
| D10 | - | - | - | PIN 2 | - | - | - |
| D11 | - | - | - | - | DOUT | - | - |
| RX | - | - | - | - | - | - | TX |
| TX | - | - | - | - | - | - | RX |



# Notes
- `Step motor 1` is used for mixing and `Step motor 2` is used for lifting up and down the container.
- `HC-05` is connected directly to the serial communications ports of the Arduino. Uploading code into the Arduino will fail when HC-05 is connected.
- `DS18B20` has a slow refresh rate, so code code slows down when it's activated.
- `Load cell`, `Pump` and `Temperature` devices are calibrated. However, it's recommended to re-calibrate them.
- Code can run without calibration equations.
- Start position of `Step motor 2` is defined as the position it's in when the Arduino turns on or resets.



# Commands
Data is received like an 5-bit `int` data type value. The first 2 bits, known as dataCode, define the function to execute. Last 3 bits, known as dataValue, define the input value when needed (only for functions 10, 20 and 30). 

- **10**: Load cell calibration. Receives `KNOWN_WEIGHT` as `dataValue` to calibrate.
- **11**: Load cell tares 100 times.
- **20**: Pump turns on with a 125 PWM and defines reference weight as `dataValue`. Up to this value the system will pump water, when weight gets to that reference point pumping will stop. 
- **21**: Turns pump off.
- **30**: Step motor 1 receives mixing value for PWM as `dataValue`.
- **31**: Turns step motor 1 off.
- **40**: Lifts down container.
- **41**: Lifts up container.
- **99**: Stops everything and returns height to start position.