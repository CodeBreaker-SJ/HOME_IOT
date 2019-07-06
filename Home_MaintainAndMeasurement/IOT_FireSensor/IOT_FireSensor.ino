
void setup() 
{
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() 
{
  //sensor 값 읽어오기
  float fire = analogRead(A0);
  
  Serial.println(fire);
  delay(100);
}
