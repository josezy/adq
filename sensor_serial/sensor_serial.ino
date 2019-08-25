#include <Arduino_JSON.h>

#define s_BH1750FVI true    // used pines: A4, A5
#define s_MQ135     true    // used pines: A0

#if s_BH1750FVI
  #include <BH1750FVI.h>
  BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
#endif

long dt = 1000; // milliseconds
long last_now = millis();
JSONVar Data;

void setup(){
  Serial.begin(115200);

  #if s_BH1750FVI
    LightSensor.begin();
    Data["BH1750FVI"]["light"]["units"] = "lumen";
  #endif
  #if s_MQ135
    Data["MQ135"]["air_ppm"]["units"] = "ppm";
  #endif
}

void loop(){
  #if s_BH1750FVI
    uint16_t lux = LightSensor.GetLightIntensity();
  #endif

  #if s_MQ135
    int air_ppm = analogRead(A0);
  #endif

  long now = millis();
  if(now - last_now > dt){
    last_now = now;

    #if s_BH1750FVI
      Data["BH1750FVI"]["light"]["value"] = (long unsigned int)lux;
    #endif
    #if s_MQ135
      Data["MQ135"]["air_ppm"]["value"] = air_ppm;
    #endif

    Serial.println(JSON.stringify(Data));
  }
}
