/* Awsome Green Spaces challange!
 *  
 *  Agnieszka Noculak, Adam Pyka
 *  
 *  Test of deep sleep - work in progress!!!
 */
 
const int sensorPin = 12; //pin used for wakeup sense
unsigned int delayS = 10; //delay in sleep in seconds

void wakeUpEvent() {
  detachInterrupt(digitalPinToInterrupt(sensorPin));
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Interrupt fired!");
}


void goSleep() {
    digitalWrite(LED_BUILTIN,LOW);//turning LED off
    delay(100); //wait a while
    attachInterrupt(digitalPinToInterrupt(sensorPin), wakeUpEvent, RISING);
    NRF_POWER->SYSTEMOFF = 1;//activating sleep mode
}


void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorPin, INPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Uaaaaaa... time to wake up!");

  while (delayS-- > 0){
    Serial.print("Oh, maybe not... Just going to sleep in ");
    Serial.println(delayS);
    delay(delayS*100);
  }
  goSleep();
}


void loop() {}
