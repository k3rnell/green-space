/* Awsome Green Spaces challange!
 *  
 *  Agnieszka Noculak, Adam Pyka
 *  
 *  Test of photoresistor
 */
 

#define SERIESRESISTOR 10000    
#define PHOTOPIN A7 
 
void setup(void) {
  Serial.begin(9600);
  while(!Serial);
}
 
void loop(void) {
  float reading;
 
  reading = analogRead(PHOTOPIN);
 
  Serial.print("Analog reading "); 
  Serial.println(reading);
 
  reading = (1023 / reading)  - 1;     // (1023/ADC - 1) 
  reading = SERIESRESISTOR / reading;  // 10K / (1023/ADC - 1)
  Serial.print("Photoresistor resistance "); 
  Serial.println(reading);
 
  delay(1000);
}
