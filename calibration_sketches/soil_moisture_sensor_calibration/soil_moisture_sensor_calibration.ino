const int MS_SENSOR_PIN = A1;
int ms_value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MS_SENSOR_PIN, INPUT);
}

void loop() {
  ms_value = analogRead(MS_SENSOR_PIN);
  Serial.println(ms_value);
  delay(1000);
}
