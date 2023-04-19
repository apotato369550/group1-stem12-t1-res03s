#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include "DHT.h"
#include "ds3231.h"

#define POWER 2
#define LED  4
#define CS_PIN 10
#define PHOTO_SENSOR A0
#define SOIL_MOISTURE A1
#define DHT_TYPE DHT11
#define DHT_PIN  8

DHT dht(DHT_PIN, DHT_TYPE);

File file;

float raw_temperature;
float corrected_temperature;
int raw_humidity;
int corrected_humidity;
int raw_light;
long corrected_light;
int raw_moisture;
float corrected_moisture;

float raw_high_temp = 42.8;
float raw_low_temp = 11.8;
float ref_high_temp = 43.1;
float ref_low_temp = 12.2;

int raw_high_humid = 95;
int raw_low_humid = 61;
int ref_high_humid = 94;
int ref_low_humid = 59;

int raw_high_soil = 1022;
int raw_low_soil = 216;
int ref_high_soil = 10;
int ref_low_soil = 0;

void setup() {
  pinMode(POWER, OUTPUT); 
  digitalWrite(POWER, HIGH);
  
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  
  pinMode(LED, OUTPUT);  
  pinMode(CS_PIN, OUTPUT);
  pinMode(PHOTO_SENSOR, INPUT);
  pinMode(SOIL_MOISTURE, INPUT);
  dht.begin();

  Serial.println("Initializing SD card...");
  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
  } else {
    Serial.println("SD card initialization failed");
  }

  file = SD.open("research.csv", FILE_WRITE);
  if (file)
  {
    Serial.println("File created successfully.");
  } else
  {
    Serial.println("Error while creating file.");
  }
  file.close();
}


void loop() {
  struct ts t;
  DS3231_get(&t);
  
  raw_temperature = dht.readTemperature();
  raw_humidity = dht.readHumidity();
  raw_light = analogRead(PHOTO_SENSOR);
  raw_moisture = analogRead(SOIL_MOISTURE);
  
  Serial.print("Time: ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(":");
  Serial.print(t.sec);
  Serial.print("\n");
  
  Serial.print("RAW Temperature: ");
  Serial.print(raw_temperature);
  Serial.print(" C \n");
  
  Serial.print("RAW Relative Humidity: ");
  Serial.print(raw_humidity);
  Serial.print(" %\n");
  
  Serial.print("RAW Light Intensity: ");
  Serial.print(raw_light);
  Serial.print(" Lux \n");
  
  Serial.print("RAW Soil Moisture Reading: ");
  Serial.print(raw_moisture);
  Serial.print(" \n");
  
  corrected_temperature = map_float(raw_temperature, raw_low_temp, raw_high_temp, ref_low_temp, ref_high_temp);
  corrected_humidity = map(raw_humidity, raw_low_humid, raw_high_humid, ref_low_humid, ref_high_humid);
  corrected_light = calibrate_light(raw_light);
  corrected_moisture = map_float(raw_moisture, raw_low_soil, raw_high_soil, ref_high_soil, ref_low_soil);

  Serial.print("CORRECTED Temperature: ");
  Serial.print(corrected_temperature);
  Serial.print(" C \n");
  
  Serial.print("CORRECTED Relative Humidity: ");
  Serial.print(corrected_humidity);
  Serial.print(" %\n");
  
  Serial.print("CORRECTED Light Intensity: ");
  Serial.print(corrected_light);
  Serial.print(" Lux \n");
  
  Serial.print("CORRECTED Soil Moisture Reading: ");
  Serial.print(corrected_moisture);
  Serial.print(" \n");

  if (t.min == 0 || t.min == 30) {
    digitalWrite(LED, HIGH);
    file = SD.open("RESEARCH.csv", FILE_WRITE);
    file.print(t.hour);
    file.print(":");
    file.print(t.min);
    file.print(":");
    file.print(t.sec);
    file.print(",");
    file.print(raw_temperature);
    file.print(",");
    file.print(corrected_temperature);
    file.print(",");
    file.print(raw_humidity);
    file.print(",");
    file.print(corrected_humidity);
    file.print(",");
    file.print(raw_light);
    file.print(",");
    file.print(corrected_light);
    file.print(",");
    file.print(raw_moisture);
    file.print(",");
    file.print(corrected_moisture);
    file.print(" \n");
    file.close();
    Serial.println("Info recorded\n");
  } else {
    digitalWrite(LED, LOW);
  }

  delay(2000);
}

long calibrate_light(long raw_reading) {
  long converted_reading = raw_reading;
  if (raw_reading <= 37) {
    converted_reading = map(converted_reading, 0, 37, 0, 106);
  } else if (raw_reading <= 61) {
    converted_reading = map(converted_reading, 0, 61, 0, 229);
  } else if (raw_reading <= 95) {
    converted_reading = map(converted_reading, 0, 95, 0, 449);
  } else if (raw_reading <= 134) {
    converted_reading = map(converted_reading, 0, 134, 0, 760);
  } else if (raw_reading <= 287) {
    converted_reading = map(converted_reading, 0, 287, 0, 2800);
  } else if (raw_reading <= 322) {
    converted_reading = map(converted_reading, 0, 322, 0, 4580);
  } else if (raw_reading <= 417) {
    converted_reading = map(converted_reading, 0, 417, 0, 9780);
  } else if (raw_reading <= 461) {
    converted_reading = map(converted_reading, 0, 461, 0, 17200);
  } else if (raw_reading <= 495) {
    converted_reading = map(converted_reading, 0, 495, 0, 22000);
  } else if (raw_reading <= 523) {
    converted_reading = map(converted_reading, 0, 523, 0, 44000);
  } else if (raw_reading <= 614) {
    converted_reading = map(converted_reading, 0, 614, 0, 66000);
  } else if (raw_reading <= 728) {
    converted_reading = map(converted_reading, 0, 728, 0, 100000);
  } else if (raw_reading <= 846) {
    converted_reading = map(converted_reading, 0, 846, 0, 230000);
  } else {
    converted_reading = map(converted_reading, 0, 1000, 0, 500000);
  }
  return converted_reading;
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
