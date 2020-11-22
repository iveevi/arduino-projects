void setup()
{
  Serial.begin(9600);
  Serial.print("root$ ");
}

void loop()
{
  if (Serial.available() > 0) {
    switch (process(Serial.read())) {
    case 0:
      break;
    case 1:
      Serial.print("root$ ");
      break;
    default:
      break; 
    }
  }
}

int process(char c)
{
  switch (c) {
  case 13:
    Serial.println();
    return 1;
  case 127:
    Serial.write('\b');
    Serial.write(' ');
    Serial.write('\b');
    break;
  default:
    if (!isprint(c)) {
      Serial.print("[");
      Serial.print((byte) c);
      Serial.print("]");
      
      break;
    }

    Serial.print(c);
    break;
  }

  return 0;
}
