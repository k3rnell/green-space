/* Awsome Green Spaces challange!
 *  
 *  Agnieszka Noculak, Adam Pyka
 *  
 *  Bench occupation by photoresistor with LoRa communication
 */
 
#include "loramodem.h"
#include "keys.h"

#define PHOTOSERIESRESISTOR 10000 //value of the photoresistor devider series resistor
#define PHOTOPIN A7

#define THERMOSERIESRESISTOR 10000 //value of the thermistor devider series resistor
#define THERMOPIN A6
#define THERMISTORNOMINAL 10000 //thermistor resistance in nominal temperature
#define TEMPERATURENOMINAL 25 //temperature in nominal resistance (almost always 25 C)
#define BCOEFFICIENT 3950 //beta coefficient of the thermistor (usually 3000-4000)

#define PIRPIN D12
#define LIGHTLIMIT 400 //limit of light above which the bench seems to be occupied
#define NUMSAMPLES 20 //averaging

LoRaWANModem modem;
Status join_state;

float temperatureValue;
bool benchOccupation;

float getPhotoRawValue(void) { //returns the light
  volatile float reading = 0;
  // take N samples in a row, with a slight delay and average
  for (int i=0; i< NUMSAMPLES; i++) {
   reading += analogRead(PHOTOPIN);
   delay(10);
  }
  reading /= NUMSAMPLES;
  reading = (1023 / reading)  - 1;     // (1023/ADC - 1) 
  reading = PHOTOSERIESRESISTOR / reading;  // 10K / (1023/ADC - 1)
  return reading;
}

float getThermistorRawValue(void) {
  volatile float reading = 0;
  // take N samples in a row, with a slight delay and average
  for (int i=0; i< NUMSAMPLES; i++) {
   reading += analogRead(THERMOPIN);
   delay(10);
  }
  reading /= NUMSAMPLES;
  reading = (1023 / reading)  - 1;     // (1023/ADC - 1) 
  reading = THERMOSERIESRESISTOR / reading;  // 10K / (1023/ADC - 1)
  return reading;
}

float getAirTemperature(float raw) {
  float steinhart;
  steinhart = raw / THERMISTORNOMINAL;         // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C
  return steinhart;
}

bool isBenchOccupated(float raw) {
  if (raw >= LIGHTLIMIT) return true;
  return false;
}

bool isSomeoneInArea(void) {
  if (digitalRead(PIRPIN)) return true;
  return false;
}


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Awsome Green Spaces! Let's start!");

  pinMode(PIRPIN, INPUT);

//  modem.begin();
//  modem.info();
//  join_state = modem.join(appeui, appkey);
}


void loop() {
  while(!isSomeoneInArea());
  temperatureValue = getAirTemperature(getThermistorRawValue());
  benchOccupation = isBenchOccupated(getPhotoRawValue());
  
  Serial.println("Yeah! There is someone in the area!");
  Serial.print("Air temperature: ");
  Serial.println(temperatureValue);
  Serial.print("Bench occupation: ");
  Serial.println(benchOccupation);

  uint8_t payload[4];
  
/* Frame to be send via LoRaWAN
 *  
 * -------------------------------------------------------------------------------
 * | bench occupation | temperature sign | temp. mantissa | temp. characteristic |
 * | 1 - occupied     | 1 - temp < 0     |                                       |
 * | 0 - not occupied | 0 - temp >= 0    |                                       |
 * -------------------------------------------------------------------------------
 * 
 * Example for occupied bench with -2.50*C:
 * ------------------
 * | 1 | 1 | 2 | 50 |
 * ------------------
 */  
  
  payload[0] = benchOccupation;
  payload[1] = 0;
  if (temperatureValue > 0) payload[1] = 1;
  payload[2] = (uint8_t) temperatureValue;
  payload[3] = (uint8_t) ((temperatureValue - (uint8_t)temperatureValue)*100);
  
  if (join_state == OK) {
    Serial.println("modem.send");
    modem.send(payload, 4);
  }

  delay(30000); // not to overload LoRa network
}
