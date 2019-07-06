void setup() 
{
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() 
{
  //sensor 값 읽어오기
  float gas = analogRead(A0);
  Serial.println(gas);
  delay(100);
}
