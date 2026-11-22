// Include the Wire library to enable I2C communication between Arduino and connected devices.
#include <Wire.h>
// Include the LiquidCrystal_I2C library to control an LCD using the I2C communication interface.
#include <LiquidCrystal_I2C.h>

// Create a 16-column, 2-row LCD object at I2C address 0x27.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Arduino pins connected to the ultrasonic sensor's TRIG and ECHO pins.
const uint8_t TRIG_PIN = 6;
const uint8_t ECHO_PIN = 7;

// Stop waiting for an echo after 30 ms so the program cannot wait indefinitely.
const unsigned long ECHO_TIMEOUT_US = 30000UL;

// Write text to one LCD row and pad the rest with spaces. Padding removes
// leftover characters if the new message is shorter than the previous one.
void printLcdLine(uint8_t row, const char *text)
{
  lcd.setCursor(0, row);  // Start at the first column of the requested row.
  lcd.print(text);

  // Fill any unused columns (up to 16) with spaces.
  for (uint8_t i = strlen(text); i < 16; ++i)
  {
    lcd.print(' ');
  }
}

void setup()
{
  // The Arduino sends a pulse on TRIG and listens for a returning pulse on ECHO.
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);  // Keep the sensor trigger inactive at startup.
  pinMode(ECHO_PIN, INPUT);

  // Start the LCD and turn on its backlight so the text is visible.
  lcd.init();
  lcd.backlight();

  // Show a brief startup message, then clear the display for measurements.
  printLcdLine(0, "Ultrasonic");
  printLcdLine(1, "Initializing...");
  delay(1500);
  lcd.clear();
}

void loop()
{
  // Send a short trigger pulse to ask the HC-SR04 to measure distance.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure how long ECHO stays HIGH. The timeout prevents an endless wait.
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0)
  {
    // No echo arrived before the timeout, so show a no-reading message.
    printLcdLine(0, "Out of range");
    printLcdLine(1, "No echo");
  } 
  else
  {
    // Sound travels to the object and back, so divide the travel distance by 2.
    // 0.0343 cm/us is the approximate speed of sound used for this estimate.
    const float distanceCm = duration * 0.0343f / 2.0f;

    // Convert the distance to text with one digit after the decimal point.
    char value[17];
    dtostrf(distanceCm, 6, 1, value);

    // Display the label on the first row and the measured value on the second.
    printLcdLine(0, "Distance:");
    char line[17];
    snprintf(line, sizeof(line), "%s cm", value);
    printLcdLine(1, line);
  }

  // Pause briefly between measurements to keep the display easy to read.
  delay(500);
}

