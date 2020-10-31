/* Awsome Green Spaces challange!
 *  
 *  Agnieszka Noculak, Adam Pyka
 *  
 *  Test of thermistor temperature readout
 */

#define THERMISTORPIN A7         
#define THERMISTORNOMINAL 10000 // resistance at 25 degrees C    
#define TEMPERATURENOMINAL 25   // temp. for nominal resistance (almost always 25 C)
#define NUMSAMPLES 5 //samples to avarage
#define BCOEFFICIENT 3950 // beta coefficient of the thermistor (usually 3000-4000)
#define SERIESRESISTOR 10000    
 
int samples[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  while(!Serial);
}
 
void loop(void) {
  uint8_t i;
  float average;
 
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  Serial.print("Average analog reading "); 
  Serial.println(average);
  
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
  
  delay(1000);
}
