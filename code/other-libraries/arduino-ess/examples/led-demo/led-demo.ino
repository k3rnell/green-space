// This example shows how to control the LEDs on the ESS
//
// *Important*: this is configured for boards following the Arduino Uno footprint.
// If your board has a different layout - e.g. ESP8266, ESP32, SAMD21 - please
// configure LED pins to match your layout!

#include <sensirion_ess.h>

// This is configured for Arduino Uno based boards; if you have a different board,
// please adapt the pins to match your setup
#define LED_RED SensirionESSLeds::ARDUINO_UNO_LED_RED_PIN
#define LED_YEL SensirionESSLeds::ARDUINO_UNO_LED_YEL_PIN
#define LED_GRN SensirionESSLeds::ARDUINO_UNO_LED_GRN_PIN

SensirionESSLeds essLeds(LED_RED,
                         LED_YEL,
                         LED_GRN);

int repetitions  =    3;
int fadeDelay    =    5;
int stepDelay    =  100;
int sectionDelay = 1000;

void setup()
{
}

void loop()
{
  // -- SensirionESSLeds::setLedRYG()
  for (int i = 0; i < repetitions; ++i) {
    essLeds.setLedRYG(1, 0, 0); delay(stepDelay);
    essLeds.setLedRYG(0, 1, 0); delay(stepDelay);
    essLeds.setLedRYG(0, 0, 1); delay(stepDelay);
  }
  essLeds.setLedRYG(0, 0, 0);
  delay(sectionDelay);

  // -- manual access
  for (int i = 0; i < repetitions; ++i) {
    digitalWrite(LED_RED, HIGH); delay(stepDelay);
    digitalWrite(LED_YEL, HIGH); delay(stepDelay);
    digitalWrite(LED_GRN, HIGH); delay(stepDelay);
    digitalWrite(LED_RED,  LOW); delay(stepDelay);
    digitalWrite(LED_YEL,  LOW); delay(stepDelay);
    digitalWrite(LED_GRN,  LOW); delay(stepDelay);
  }
  essLeds.setLedRYG(0, 0, 0);
  delay(sectionDelay);

  // -- analogWrite() - requires pins 9-11 to be PWM pins
  for (int i = 0; i < repetitions; ++i) {
    for (int j = 0; j < 255; ++j) {
      analogWrite(LED_RED, j);
      analogWrite(LED_YEL, j);
      analogWrite(LED_GRN, j);
      delay(fadeDelay);
    }
    for (int j = 255; j >= 0; --j) {
      analogWrite(LED_RED, j);
      analogWrite(LED_YEL, j);
      analogWrite(LED_GRN, j);
      delay(fadeDelay);
    }
  }
  essLeds.setLedRYG(0, 0, 0);
  delay(sectionDelay);
}
