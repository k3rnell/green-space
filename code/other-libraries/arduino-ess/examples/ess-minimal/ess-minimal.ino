// this is a minimal version without error checking
// this is meant purely to exaplain the API, and its use is not recommended
// to use this in a serious application

#include <sensirion_ess.h>

SensirionESS ess;

void setup()
{
  Serial.begin(9600);
  delay(1000); // let console settle

  ess.initSensors();
}

void loop() {
  ess.measureIAQ(); // measure first to ensure proper timing
  ess.measureRHT();

  Serial.print(ess.getTemperature());  Serial.print(" ");
  Serial.print(ess.getHumidity());     Serial.print(" ");
  Serial.print(ess.getTVOC());         Serial.print(" ");
  if (ess.getProductType() == SensirionESS::PRODUCT_TYPE_SGP30) {
    Serial.print(ess.getECO2());
  }
  Serial.print("\n");

  delay(ess.remainingWaitTimeMS());
}
