const int PIR_SENSOR_OUTPUT_PIN = 2;

void setup() {
  pinMode(PIR_SENSOR_OUTPUT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), pir, FALLING);  
  Serial.begin(9600); 
  delay(2000); 
}

void loop() {
}

void pir(){
  Serial.println("Object Detected");
}
