const int relayPin = 8;

void setup() 
{
  pinMode(relayPin, OUTPUT);
}

void loop() 
{
  digitalWrite(relayPin, HIGH);  
  delay(5000);                     
  digitalWrite(relayPin, LOW);  
  delay(5000);                  
}
