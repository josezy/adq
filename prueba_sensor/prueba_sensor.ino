
/*
  This example uses only 4 wires to test the BH1750 Light sensor
 
  Connecting the sensor to a Arduino UNO:
  VCC  <-> 3V3
  GND  <-> GND
  SDA  <-> A4/SDA 
  SCL  <-> A5/SCL
*/

#include <BH1750FVI.h>
#include <Arduino_JSON.h>
#define INPUT_SIZE 30


// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

long dt=60; //MilliSecons
bool SendJson=false;
long time_to_send=millis();
JSONVar Sensor,Data,Units;
void setup() 
{
  Serial.begin(115200);
  LightSensor.begin();  
  Units["luz"]="LUMENS";
  Sensor["ts"]=1; 
  Sensor["luz"]=1;
  Sensor["units"]=Units;
  Data["BH1750FVI"]=Sensor;
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  if(millis()-time_to_send>dt){  
    Serial.flush();  
    time_to_send=millis();
    Sensor["luz"]=(String) lux;
    Data["BH1750FVI"]=Sensor;
    Serial.println(JSON.stringify(Data));
    
  }
  
}
