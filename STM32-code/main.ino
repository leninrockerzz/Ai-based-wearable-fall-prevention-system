void setup()
{
  Serial.begin(9600);
  delay(100);
  pinMode(PB9,INPUT_PULLUP);
  delay(10);
}
int flag = 0;
void loop()
{
  if(digitalRead(PB9) == LOW && flag == 0)
  {
    Serial.println('a');
    delay(100);
    flag = 1;
  }
   if(digitalRead(PB9) == HIGH && flag == 1)
  {
    Serial.println('b');
    delay(100);
    flag = 0;
  }
}
