
const int buzzerPin = 9;

void setup() 
{
  pinMode(buzzerPin, OUTPUT);
}

void loop() 
{
  for(int i = 0 ; i < 256; i++)
  {
    analogWrite(buzzerPin, i);
    delay(100);
  }
}
