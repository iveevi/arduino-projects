// Trigger pin of ultrasonic sensor
const int in_ping = 7;

// Echo pin of ultrasonic sensor
const int in_echo = 6;

// Duration
long dt;

// Computed distance
double dx;

// Sliding buffer
const int buf_size = 10;

struct bin {
  // Array
  double *window;

  // Current index
  int index;

  // The avergae value
  double average;
} buf;

// Setup
void setup()
{
  // Setup serial
  Serial.begin(9600);

  // Setup pins
  pinMode(in_ping, OUTPUT);
  pinMode(in_echo, INPUT);

  // Setup sliding buffer
  buf.window = new double[buf_size];

  for (int i = 0; i < buf_size; i++)
    buf.window[i] = 0.0;

  buf.index = 0;

  buf.average = 0.0;
}

void loop()
{
  // Get the data
  digitalWrite(in_ping, LOW);
  delayMicroseconds(5);
  digitalWrite(in_ping, HIGH);
  delayMicroseconds(5);
  digitalWrite(in_ping, LOW);
  
  dt = pulseIn(in_echo, HIGH);
  
  dx = (double) dt / 29.0 / 2.0;

  // Adjust the buffer
  buf.average = (buf.average * buf_size - buf.window[buf.index] + dx)/buf_size;

  buf.window[buf.index] = dx;

  buf.index = (buf.index + 1) % buf_size;

  // Display data
  Serial.print(dx);
  Serial.print("\t");
  Serial.println(buf.average);
}
