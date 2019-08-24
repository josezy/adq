
/*
  This example uses only 4 wires to test the BH1750 Light sensor
 
  Connecting the sensor to a Arduino UNO:
  VCC  <-> 3V3
  GND  <-> GND
  SDA  <-> A4/SDA 
  SCL  <-> A5/SCL
*/

#include <BH1750FVI.h>
#define INPUT_SIZE 30


// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

int dt=100; //MilliSecons
bool SendJson=false;
long main_delay=millis();
void setup() 
{
  Serial.begin(115200);
  LightSensor.begin();  
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  
  if (Serial.available()>0){
    String json="{";
    char input[INPUT_SIZE + 1];
    byte size = Serial.readBytesUntil('|',input, INPUT_SIZE);
    input[size] = 0;
    char* command = strtok(input, "&");
    
    if(strcmp(command,"BH1750FVI")==0){
      SendJson=true;
      json=json+"\""+command+"\":"+(String) lux;
    }else if(strcmp(command,"NONE")==0){
      SendJson=true;
      json=json+"\""+command+"\":\"NON\"";
    }
    
    while (command != 0)
    {
        command = strtok(0, "&");
        if(strcmp(command,"BH1750FVI")==0){
          json=json+",\""+command+"\":"+(String) lux;
        }else if(strcmp(command,"NONE")==0){
          json=json+",\""+command+"\":\"NON\"";
        }
    }
    
    if(SendJson){      
      json=json+"}";
      Serial.println(json);
      SendJson=false;
    }
  }
  if(millis()-main_delay
  
}
