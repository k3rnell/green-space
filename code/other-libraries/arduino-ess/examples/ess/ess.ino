// we need to include sensirion_ess.h
#include <sensirion_ess.h>

//  Create an instance of SensirionESS
SensirionESS ess;

void setup()
{
  Serial.begin(9600);
  delay(1000); // let console settle

  // First step is to initialize the sensors; this should only fail if
  // the board is defect, or the connection isn't working. Since there's nothing
  // we can do if this fails, the code will loop forever if an error is detected
  if (ess.initSensors() != 0) {
    Serial.print("Error while initializing sensors: ");
    Serial.print(ess.getError());
    Serial.print("\n");
    while (1) { // loop forever
      delay(1000);
    }
  }

  // The SGP sensor has product type information and feature set stored
  // the following code reads it out, and prints it to the serial console.
  // This is purely to demo the function calls, and is not necessary to operate
  // the sensor
  int type = ess.getProductType();
  int fsVersion = ess.getFeatureSetVersion();

  Serial.print((type == SensirionESS::PRODUCT_TYPE_SGP30) ? "SGP30" : "SGPC3");
  Serial.print(" detected, running feature set version ");
  Serial.println(fsVersion);
}

void loop() {
  float temp, rh, tvoc, eco2 = -1;

  // we'll start by triggering a measurement of the VOC/CO2 sensor;
  // it's important to do this first to make sure sleep timing is
  // correct. If the command succeeds, the local variables will
  // be set to the values we just read; if it fails, they'll be -1
  if (ess.measureIAQ() != 0) {
    Serial.print("Error while measuring IAQ: ");
    Serial.print(ess.getError());
    Serial.print("\n");
  } else {
    tvoc = ess.getTVOC();
    eco2 = ess.getECO2(); // SGP30 only
  }

  // next, we'll trigger the humidity and temperature measurement
  if (ess.measureRHT() != 0) {
    Serial.print("Error while measuring RHT: ");
    Serial.print(ess.getError());
    Serial.print("\n");
  } else {
    temp = ess.getTemperature();
    rh = ess.getHumidity();
  }

  // finally, let's print those to the serial console
  Serial.print(temp);
  Serial.print(" ");
  Serial.print(rh);
  Serial.print(" ");
  Serial.print(tvoc);
  Serial.print(" ");
  if (ess.getProductType() == SensirionESS::PRODUCT_TYPE_SGP30) {
    Serial.print(eco2);
  }

  Serial.print("\n");

  // and then, we'll use remainingWaitTimeMS() to ensure the correct
  // Measurement rate
  delay(ess.remainingWaitTimeMS());
}
