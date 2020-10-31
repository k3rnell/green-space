// this example sends data from the ESS to dweet.io, which can then be visualized
// by services like freeboard.io
//
// For this to work, update the <THING> in the DWEET_URL below
//
// This also expects the Yun to have an internet connection, either through
// Wifi or Ethernet
//
// The data is sent asynchronously to make sure we can keep up with the
// SGP readout rate, even if we hit an issue on the upload

#include <sensirion_ess.h>

#include <HttpClient.h>

SensirionESS ess;
HttpClient client;

// ADJUST <THING> HERE with your dweet.io thing:
const char* DWEET_URL = "http://dweet.io/dweet/for/<THING>";

// Let's limit dweet requests to 0.5 Hz
long lastDweetTimestamp;
const int DWEET_THRESHOLD = 2000;

// We'll set a CO2 threshold for the human precense detection
const int CO2_THRESHOLD = 1000;

// for the SGP, the first n values are always default values; we'll count
// samples, and then signal to the dashboard that we can expect measured
// data from now on
int sampleCount = 0;
const int INIT_SAMPLE_THRESHOLD = 15;

// helper function to create the dweet.io URL
String createRequest(float tvoc, float eco2, float temp, float rh,
    int initState, int perspres)
{
  String s = DWEET_URL;
  s += "?temp=";      s += temp;
  s += "&rh=";        s += rh;
  s += "&tvoc=";      s += tvoc;
  s += "&eco2=";      s += eco2;
  s += "&perspres=";  s += perspres;
  s += "&initstate="; s += initState;
  return s;
}

void setup()
{
  SerialUSB.begin(9600);
  while (!SerialUSB) {} // wait for a serial connection

  SerialUSB.print("Connecting to bridge... ");
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  SerialUSB.println("done!");

  if (ess.initSensors() != 0) {
    SerialUSB.print("Error while initializing sensors: ");
    SerialUSB.print(ess.getError());
    SerialUSB.print("\n");
    while (1) { // loop forever
      delay(1000);
    }
  }

  lastDweetTimestamp = millis() - DWEET_THRESHOLD;
}

void loop() {
  float temp, rh, tvoc, eco2 = -1;

  if (ess.measureIAQ() != 0) {
    SerialUSB.print("Error while measuring IAQ: ");
    SerialUSB.print(ess.getError());
    SerialUSB.print("\n");
  } else {
    tvoc = ess.getTVOC();
    eco2 = ess.getECO2(); // SGP30 only
  }

  // only increment until it reaches the threshold
  if (sampleCount < INIT_SAMPLE_THRESHOLD) {
    ++sampleCount;
  }

  if (ess.measureRHT() != 0) {
    SerialUSB.print("Error while measuring RHT: ");
    SerialUSB.print(ess.getError());
    SerialUSB.print("\n");
  } else {
    temp = ess.getTemperature();
    rh = ess.getHumidity();
  }

  int personPresent = (eco2 > CO2_THRESHOLD) ? 1 : 0;
  int initState = (sampleCount >= INIT_SAMPLE_THRESHOLD);

  // Send data to dweet.io
  long tNow = millis();
  if ((tNow  - lastDweetTimestamp) >= DWEET_THRESHOLD) {
    if (client.ready()) {
      lastDweetTimestamp = tNow;
      String request = createRequest(tvoc, eco2, temp, rh, initState, personPresent);
      client.getAsynchronously(request);

      SerialUSB.println(request);
    }
  }

  delay(ess.remainingWaitTimeMS());
}
