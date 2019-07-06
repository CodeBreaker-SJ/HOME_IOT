
//핀번호 설정
const int color_R_Pin = 9;
const int color_G_Pin = 11;
const int color_B_Pin = 10;

//RGB 변수 설정
int color_R = 0;
int color_G = 0;
int color_B = 0;

void setup() 
{
  //핀 설정
  pinMode(color_R_Pin, OUTPUT);
  pinMode(color_G_Pin, OUTPUT);
  pinMode(color_B_Pin, OUTPUT);

  //초기 값 설정
  analogWrite(color_R_Pin, color_R);
  analogWrite(color_G_Pin, color_G);
  analogWrite(color_B_Pin, color_B);
}

void loop() 
{
  //붉은색 농도 0~255
  for(int i = 0 ; i < 255; i++)
  {
    color_R = i;
    analogWrite(color_R_Pin, color_R);
    analogWrite(color_G_Pin, 0);
    analogWrite(color_B_Pin, 0);
    delay(10);
  }

  //붉은색 농도 0~255
  for(int i = 0 ; i < 255; i++)
  {
    color_G = i;
    analogWrite(color_R_Pin, 0);
    analogWrite(color_G_Pin, color_G);
    analogWrite(color_B_Pin, 0);
    delay(10);
  }

  //붉은색 농도 0~255
  for(int i = 0 ; i < 256; i++)
  {
    color_B = i;
    analogWrite(color_R_Pin, 0);
    analogWrite(color_G_Pin, 0);
    analogWrite(color_B_Pin, color_B);
    delay(10);
  }
}
