#include <Arduino_JSON.h>

#define s_BH1750FVI false    //
#define s_MQ135     true     // A0
#define s_KS0196    false    //
#define s_AM2302    true     // DD2

#if s_BH1750FVI
  #include <BH1750FVI.h>
  BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
#endif

#if s_KS0196
  int ks0196_analog_pin = A1;
  int ks0196_digital_pin = DD0;
  int ks0196_t1 = 280;
  int ks0196_t2 = 9620;
  int N = 1;
  float mean = 0; 
#endif

#if s_AM2302
  #include <DHT.h>
  #define DHTPIN DD2
  DHT dht(DHTPIN, DHT22);
#endif

long dt = 1000; // milliseconds
long last_now = millis();
JSONVar data;

void setup(){
  Serial.begin(115200);

  #if s_BH1750FVI
    LightSensor.begin();
    data["BH1750FVI"]["light"]["units"] = "lumen";
  #endif
  #if s_MQ135
    data["MQ135"]["air_ppm"]["units"] = "ppm";
  #endif
  #if s_AM2302
    dht.begin();
    data["AM2302"]["humidity"]["units"] = "%";
    data["AM2302"]["temperature"]["units"] = "ºC";
  #endif
  #if s_KS0196
    pinMode(ks0196_analog_pin, INPUT);
    pinMode(ks0196_digital_pin, OUTPUT);
    data["KS0196"]["air_ppm"]["units"] = "ppm";
  #endif
}

void loop(){
  #if s_BH1750FVI
    uint16_t lux = LightSensor.GetLightIntensity();
  #endif

  #if s_MQ135
    int air_ppm = map(analogRead(A0), 0, 1023, 0, 100);
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

  #if s_AM2302
    // Readings take about 250 milliseconds
    // and may be up to 2 seconds 'old' (slow sensor)
    float am2302_humidity = dht.readHumidity();
    float am2302_temperature = dht.readTemperature(); // celsius
  #endif

  long now = millis();
  if(now - last_now > dt){
    last_now = now;

    #if s_BH1750FVI
      data["BH1750FVI"]["light"]["value"] = (long unsigned int)lux;
    #endif
    #if s_MQ135
      data["MQ135"]["air_ppm"]["value"] = air_ppm;
    #endif
    #if s_KS0196
      data["KS0196"]["air_ppm"]["value"] = mean / N;
    #endif
    #if s_AM2302
      float h = isnan(am2302_humidity) ? -1 : am2302_humidity;
      float t = isnan(am2302_temperature) ? -1 : am2302_temperature;
      data["AM2302"]["humidity"]["value"] = (int)h;
      data["AM2302"]["temperature"]["value"] = (int)t;
    #endif

    Serial.println(JSON.stringify(data));
  }
}
