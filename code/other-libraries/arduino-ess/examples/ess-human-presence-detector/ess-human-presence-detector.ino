#include <sensirion_ess.h>

SensirionESS ess;

// this example is using the SGP30's eCO2 signal to detect human presence
//
// During warmup, the yellow LED is on to show that the demo is not ready yet
// After warmup, the yellow LED will turn off, and the red LED will turn on/off
// depending on whether the measured eCO2 signal is above the threshold  or not
//
// Note that this is meant for demonstrator purposes only, as the threshold
// for the "presence" signal is that the signal is 20% (1.2x) higher than
// the initial reading (initial eCO2 signal after warmup)
//
// Limitations: the threshold will not adjust until the board is rebooted.
// As such, if the eCO2 in the room changes, the lowest values may go
// beyond the threshold already. In that case, simply reset the board

#define SGP_WARMUP_COUNT 20

void setup()
{
  Serial.begin(9600);
  delay(1000); // let console settle

  ess.initSensors();
  if (ess.getProductType() != SensirionESS::PRODUCT_TYPE_SGP30) {
    Serial.print("SGP30 is required for this demo. Exiting\n");
    while (1) {
      delay(1000); // wait forever
    }
  }
  digitalWrite(SensirionESSLeds::ARDUINO_UNO_LED_YEL_PIN, HIGH);
}

void loop() {
  static int measurementCount = 0;
  static double eco2_threshold = 380;

  ess.measureIAQ();

  Serial.print(ess.getECO2());
  Serial.print(" ");
  Serial.print(eco2_threshold);
  Serial.print("\n");

  if (measurementCount > SGP_WARMUP_COUNT) {
    if (ess.getECO2() > eco2_threshold) {
      digitalWrite(SensirionESSLeds::ARDUINO_UNO_LED_RED_PIN, HIGH);
    } else {
      digitalWrite(SensirionESSLeds::ARDUINO_UNO_LED_RED_PIN, LOW);
    }
  } else {
    if (measurementCount == SGP_WARMUP_COUNT) {
      digitalWrite(SensirionESSLeds::ARDUINO_UNO_LED_YEL_PIN, LOW);
      eco2_threshold = ess.getECO2() * 1.2;

      Serial.print("Threshold set at: ");
      Serial.println(eco2_threshold);
    }
    measurementCount++;
  }

  delay(ess.remainingWaitTimeMS());
}
