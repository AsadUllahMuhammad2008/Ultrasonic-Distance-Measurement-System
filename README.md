# Ultrasonic Distance Measurement System
An Arduino Uno project that measures the distance to an object with an **HC-SR04 ultrasonic sensor** and displays the result on a **16×2 I²C LCD**.

The Arduino sends a short trigger pulse, measures the time taken for the echo to return, converts that time into centimetres, and updates the LCD approximately every half-second.
## Link
https://app.cirkitdesigner.com/project/fd6a2ca5-a0b5-43da-8b9f-3155bc9a5fd7

## Features
- Measures object distance using an HC-SR04 ultrasonic sensor.
- Displays the result in centimetres with one decimal place.
- Uses an I²C LCD, so the display needs only two Arduino data pins.
- Uses a 30 ms echo timeout to avoid waiting indefinitely when no echo is received.
- Displays a No echo message when a reading times out.
- Pads LCD lines with spaces to remove characters left over from longer messages.
## Components
| Component | Purpose |
|------|----------|
|Arduino Uno | Controls the sensor and LCD, and calculates distance|
|HC-SR04 ultrasonic sensor | Sends an ultrasonic burst and receives its echo|
|16×2 LCD with I²C interface | Displays the measured distance or timeout message|
|Breadboard and jumper wires | Provide temporary circuit connections|
|USB cable or suitable regulated | power source	Powers the Arduino and circuit|


## Wiring
## HC-SR04 to Arduino Uno
|HC-SR04 pin | Arduino Uno pin|
|-----------|-----------|
|VCC | 5V|
|TRIG | D6|
|ECHO | D7|
|GND | GND|


## I²C LCD to Arduino Uno
|I²C LCD pin | Arduino Uno pin|
|-------|------|
|VCC | 5V|
|GND | GND|
|SDA | A4 (SDA)|
|SCL | A5 (SCL)|


All modules must share a common ground. The sketch uses LCD I²C address 0x27. Some LCD backpacks use a different address; check the module or run an I²C scanner if the display does not respond.
## Software requirements
- Arduino IDE
- Arduino Uno board support
- Wire library (included with the Arduino AVR core)
- A compatible LiquidCrystal_I2C library

The sketch is in [`Ultrasonic_Distance_Measurement_System.ino`](Ultrasonic_Distance_Measurement_System.ino). In Arduino IDE:
1. Install or open Arduino IDE.
2. Install a compatible LiquidCrystal_I2C library from Library Manager.
3. Open Ultrasonic_Distance_Measurement_System.ino.
4. Select Arduino Uno under Tools → Board.
5. Select the correct serial/USB port.
6. Verify the sketch and upload it to the board.
7. Open the LCD and place an object in front of the sensor to observe the distance reading.

Different LiquidCrystal_I2C library variants expose slightly different initialization methods. This sketch uses lcd.init() and requires a compatible variant.
## How it works
**1. Initialize:** Configure D6 as an output and D7 as an input. Initialize the LCD and show a startup message.

**2. Trigger:** Keep TRIG LOW briefly, drive it HIGH for 10 microseconds, and then return it LOW.

**3. Measure echo:** pulseIn() measures how long ECHO stays HIGH, with a maximum wait of 30,000 microseconds.

**4. Handle timeout:** If pulseIn() returns 0, display Out of range and No echo.

**5. Calculate distance:** Divide the sound's round-trip distance by two:
```
   distance_cm = echo_duration_us × 0.0343 / 2
```
   The constant 0.0343 cm/µs is an approximate speed of sound at ordinary room temperature.
   
**6. Display:** Format the result to one decimal place and print it on the second LCD row.

**7. Repeat:** Wait 500 ms and take another measurement.
## Example output
When a valid echo is received, the LCD displays a layout similar to:
```
Distance:
  23.4 cm
```
When no echo is received before the timeout:
```
Out of range
No echo
```
The values above illustrate the display format; they are not recorded test results.
## Important implementation details
- TRIG_PIN is set to digital pin 6 and ECHO_PIN to digital pin 7.
- ECHO_TIMEOUT_US is 30000UL (30 ms).
- The measured pulse represents the sound's trip to the object and back, so the calculation divides by two.
- dtostrf() formats the floating-point distance as text with one decimal place.
- snprintf() constructs the LCD text line, and printLcdLine() fills unused columns with spaces.
- A timeout does not prove that an object is physically out of range. A disconnected sensor, incorrect wiring, poor reflection, or other fault can also cause no echo.
## Troubleshooting
|Symptom | Checks|
|--------|-------|
|LCD is blank | Check LCD power, common GND, SDA=A4, SCL=A5, contrast/backpack adjustment, and I²C address.|
|LCD backlight is on but no text appears | Confirm the library is compatible with lcd.init() and that the address is correct.|
|No echo appears continuously | Check sensor VCC and GND, TRIG=D6, ECHO=D7, and ensure the target can reflect the ultrasonic signal.|
|Distance is unstable | Keep the sensor still, use a broad flat target, avoid angled/soft surfaces, and check the power and jumper connections.|
|Board resets or readings are erratic | Use a stable supply and verify that the modules share a common ground.|


## Limitations
- The speed-of-sound constant is fixed, so temperature and environmental changes can affect accuracy.
- Object material, angle, size, and surface shape can affect the returned echo.
- pulseIn() is a blocking measurement function; the sketch waits for the echo or timeout before continuing.
- The project reports one sensor reading at a time and does not perform filtering, averaging, or calibration.
- The 30 ms timeout is a software limit, not a guarantee of the sensor's usable measurement range.
## Possible improvements
- Average several readings or use a median filter to reduce fluctuation.
- Add a configurable warning threshold and buzzer/LED indicator.
- Add temperature compensation for the speed of sound.
- Add minimum/maximum distance validation and a clearer sensor-fault status.
- Store measurements or transmit them over serial, Bluetooth, or Wi-Fi.
## Repository structure
```
Ultrasonic_Distance_Measurement_System/
├── Ultrasonic_Distance_Measurement_System_Circuit_Diagram.png
├── Ultrasonic_Distance_Measurement_System.ino
├── README.md
└── PROJECT_REPORT.md
```

