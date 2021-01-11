#define MAX_CMD_LENGTH 1024

void setup()
{
  Serial.begin(9600);
  Serial.print("root$ ");

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

bool s = false;
void set()
{
  if (s)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
  
  s = !s;
}

char cmd[MAX_CMD_LENGTH];

void loop()
{
  if (Serial.available() > 0) {
    switch (process(Serial.read())) {
    case 0:
      break;
    case 1:
      execute();
      Serial.print("root$ ");
      break;
    default:
      break; 
    }
  }
}

char exe[MAX_CMD_LENGTH];

void execute()
{
  Serial.print("cmd = \"");
  Serial.print(cmd);
  Serial.println("\"");

  int i = 0;
  while (i < MAX_CMD_LENGTH && cmd[i] != 0) {
    if (isspace(cmd[i])) {
      Serial.print("exe = \"");
      Serial.print(exe);
      Serial.println("\"");

      if (strcmp(exe, "set") == 0) {
        Serial.println("Set command detected");
      } else {
        Serial.println("Command not found.");
      }
    } else {
      exe[i] = cmd[i];
    }

    i++;
  }

  if (strcmp(exe, "set") == 0) {
    Serial.println("Set command detected");

    set();
  } else {
    Serial.println("Command not found.");
  }

  memset(cmd, 0, MAX_CMD_LENGTH);
  memset(exe, 0, MAX_CMD_LENGTH);
}

int pos = 0;

int process(char c)
{
  switch (c) {
  case 13:
    Serial.println();
    
    pos = 0;
    return 1;
  case 127:
    if (pos) {
      cmd[pos--] = 0;
      
      Serial.write('\b');
      Serial.write(' ');
      Serial.write('\b');
    }
    
    break;
  default:
    cmd[pos++] = c;
    
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
