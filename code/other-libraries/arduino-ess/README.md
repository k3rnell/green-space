## Arduino library for Sensirion Environmental Sensor Shield

This is an Arduino library for the Sensirion Environmental Sensor
Shield (ESS). It supports the following generations:

- Pre-release ESS with SHTC1 and SGPC3
- Gen 1 ESS with SHTC1 and SGP30  
- Gen 2 ESS with SHTC3 and SGPC3

### Limitations:
The following is not implemented yet:

- Persistent baseline storage/restoring
- Humidity compensation

### First steps

#### Installation
To get started with the ESS, follow the following steps:

1. make sure you have the latest Arduino IDE installed from the [Arduino website](https://www.arduino.cc/en/Main/Software)
2. Start the Arduino IDE
3. Select `"Sketch"` > `"Include Library..."` > `"Manage Libraries..."`
4. Search for `"Sensirion"`, then select `"arduino-ess"`

The same dialog can also be used to check and install updated versions of the library.

#### Hardware setup
To use the ESS with an Arduino platform, simply align the pins of the shield with the headers on your Arduino and plug it in. Alternatively, you can use the connector on the back to connect the ESS using cables. This way, the sensors can be located away from the Arduino board.

#### Running the examples
To understand how to use the library, the easiest way is to use the examples included in the library. To open an example, start the Arduino IDE, and select menu `"File" > "Examples" > "arduino-ess-v0.5.0" > "(name of the example to open)"`. Note that the version in "arduino-ess-v0.5.0" can differ if newer versions get released.

We recommend to start with the `ess` example, as it shows how to initialize the sensor and read all the data. To see the data, either use the "Serial monitor" or "Serial Plotter" in the Arduino IDE; both are available from the `Tools` menu.

After that, check out the `led-demo` example, which explains how to control the LEDs on the ESS.

#### API

We've added a lot of comments to the `ess` example, so please check out the [source code](https://github.com/Sensirion/arduino-ess/blob/master/examples/ess/ess.ino) to learn more.


Here's a minimal example without error checking for illustration purposes.
In your demos, please use error checking as shown in the `ess` example!
```c++
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
```

### Schematics

You can find the schematics for the ESS in the [ess-hardware-docs repository](https://github.com/winkj/ess-hardware-docs/blob/master/ESS_Schematic.PDF)

### Feedback
If you have questions or suggestions, please submit an issue via github.
