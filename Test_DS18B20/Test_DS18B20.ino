#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Read Temperature from DS18B20");

  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println(sensors.getTempCByIndex(0));  /* Temperature for the device 1 (index 0) */
}
