#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows

// Define the pins for the LEDs and the buzzer
const int whiteLedPin = 11;
const int redLedPin = 12;
const int greenLedPin = 13;
const int buzzerPin = 9;
 
// Define the pins for the TCS3200 color sensor
const int s0 = 4;
const int s1 = 5;
const int s2 = 6;
const int s3 = 7;
const int outPin = 8;

// Threshold values for detecting colors
const int whiteThreshold = 70; // Example threshold for white detection
const int redMin = 100, redMax = 200; // Example range for red detection
const int greenMin = 100, greenMax = 200; // Example range for green detection
const int blueMin = 100, blueMax = 200; // Example range for blue detection

const int lightGreenRedMax = 150; // Maximum red frequency for light green
const int lightGreenGreenMin = 150, lightGreenGreenMax = 250; // Range for green frequency for light green
const int lightGreenBlueMax = 150; // Maximum blue frequency for light green

void setup() {
  lcd.init(); // Initialize the LCD
  lcd.backlight();

  // Set the LED pins as outputs
  pinMode(whiteLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  // Set the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);

  // Set the TCS3200 pins as outputs or inputs as necessary
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);

  // Set the frequency scaling to 20%
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the color from the TCS3200 sensor
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  int redFrequency = pulseIn(outPin, LOW);

  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  int greenFrequency = pulseIn(outPin, LOW);

  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  int blueFrequency = pulseIn(outPin, LOW);

  // Print the frequency values to the serial monitor for debugging
  Serial.print("Red: ");
  Serial.print(redFrequency);
  Serial.print(" Green: ");
  Serial.print(greenFrequency);
  Serial.print(" Blue: ");
  Serial.println(blueFrequency);

  // Determine the detected color and light up the respective LED
  if (redFrequency < whiteThreshold && greenFrequency < whiteThreshold && blueFrequency < whiteThreshold) {
    digitalWrite(whiteLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Colour is white");
    tone(buzzerPin, 1000); // Turn the buzzer on at 1000 Hz
    
  } else if (redFrequency >= redMin && redFrequency <= redMax && greenFrequency > greenMax && blueFrequency > blueMax) {
    digitalWrite(whiteLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Colour is red");
    tone(buzzerPin, 1000); // Turn the buzzer on at 1000 Hz

  } else if (redFrequency > redMax && greenFrequency >= greenMin && greenFrequency <= greenMax && blueFrequency > blueMax) {
    digitalWrite(whiteLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Colour is green");
    tone(buzzerPin, 1000); // Turn the buzzer on at 1000 Hz

  } else if (redFrequency <= lightGreenRedMax && greenFrequency >= lightGreenGreenMin && greenFrequency <= lightGreenGreenMax && blueFrequency <= lightGreenBlueMax) {
    digitalWrite(whiteLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Colour is light");
    lcd.setCursor(0, 1);
    lcd.print("green");
    tone(buzzerPin, 1000); // Turn the buzzer on at 1000 Hz

  } else {
    digitalWrite(whiteLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Color error");
    noTone(buzzerPin); // Turn the buzzer off
  }

  delay(1000); // Wait for the pause duration
}