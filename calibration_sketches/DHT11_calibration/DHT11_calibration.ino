#include "DHT.h"

#define DHT_TYPE DHT11

const int DHT_PIN = 8;

DHT dht(DHT_PIN, DHT_TYPE);

float humidity_val;
float temp_val_c;
float heat_index_c;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  dht.begin();
}

void loop() {
  humidity_val = dht.readHumidity();
  temp_val_c = dht.readTemperature();

  if (isnan(humidity_val) && isnan(temp_val_c)) {
    Serial.println("Reading DHT sensor failed!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity_val);
  Serial.print(" %\n");

  Serial.print("Temperature: ");
  Serial.print(temp_val_c);
  Serial.print(" C \n");
  delay(2000);
}
