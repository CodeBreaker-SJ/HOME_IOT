const int soundSensorPin = 8;

void setup() 
{
  pinMode(soundSensorPin, INPUT);
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() 
{
  int sound = digitalRead(soundSensorPin);
  if (sound == HIGH)
    Serial.println("loud!!");
    
  delay(10);
}
