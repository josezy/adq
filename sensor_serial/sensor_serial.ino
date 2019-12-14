#include <Arduino_JSON.h>

#define s_BH1750FVI true    // used pines: A4, A5
#define s_MQ135     true    // used pines: A0
#define s_KS0196    true    // used pines: A1, D0

#if s_BH1750FVI
  #include <BH1750FVI.h>
  BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
#endif

#if s_KS0196
  int ks0196_analog_pin = A1;
  int ks0196_digital_pin = D0;
  int ks0196_t1 = 280;
  int ks0196_t2 = 9620;
  int N = 1;
  float mean = 0; 
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
  #if s_KS0196
    pinMode(ks0196_analog_pin, INPUT);
    pinMode(ks0196_digital_pin, OUTPUT);
    Data["KS0196"]["air_ppm"]["units"] = "ppm";
  #endif
}

void loop(){
  #if s_BH1750FVI
    uint16_t lux = LightSensor.GetLightIntensity();
  #endif

  #if s_MQ135
    int air_ppm = analogRead(A0);
  #endif

  #if s_KS0196
    for (int n = 0; n < N; n++){
      digitalWrite(ks0196_digital_pin, LOW);
      delayMicroseconds(ks0196_t1);
      mean += analogRead(ks0196_analog_pin); 
      digitalWrite(ks0196_digital_pin, HIGH); 
      delayMicroseconds(ks0196_t2);
    }
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
    #if s_KS0196
      Data["KS0196"]["air_ppm"]["value"] = mean / N;
    #endif

    Serial.println(JSON.stringify(Data));
  }
}
