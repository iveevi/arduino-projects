// Macros
#define BUF_SIZE    10
#define DELAY_SIZE  5

#define EPSILON     0.5

// Trigger pin of ultrasonic sensor
const int in_ping = 7;

// Echo pin of ultrasonic sensor
const int in_echo = 6;

// Duration
long dt;

// Computed distance
double dx;

// Sliding buffer
struct bin_1 {
  // Array of values
  double window[BUF_SIZE];

  // Array of averages
  double avgs[BUF_SIZE];

  // Current index
  int index;

  // The average value
  double average;

  // The variance of the data
  double variance;

  double rep_avg;
} buf;

struct bin_2 {
  // Array
  double window[DELAY_SIZE];

  // Current index;
  int index;

  // Last replaced value
  double replaced;
} dff;

// Misc global variables
int count = 0;

bool alarmed = false;

// Setup
void setup()
{
  // Setup serial
  Serial.begin(9600);

  // Setup pins
  pinMode(in_ping, OUTPUT);
  pinMode(in_echo, INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup sliding average buffer
  for (int i = 0; i < BUF_SIZE; i++)
    buf.window[i] = 0.0;

  for (int i = 0; i < BUF_SIZE; i++)
    buf.avgs[i] = 0.0;

  buf.index = 0;

  buf.average = 0.0;
  buf.variance = 0.0;
  buf.rep_avg = 0.0;

  // Setup sliding delayed buffer
  for (int i = 0; i < BUF_SIZE; i++)
    dff.window[i] = 0.0;

  dff.index = 0;

  dff.replaced = 0.0;
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

  // Adjust the average buffer
  double tmp = buf.average;
  double pdx = buf.window[buf.index];

  // Serial.print("Pre-Variance: ");
  // Serial.println(buf.variance);

  buf.average += (dx - pdx)/BUF_SIZE;

  /*Serial.print("Delta: ");
  Serial.println(((dx - tmp) * (dx - buf.average) - (pdx - buf.rep_avg) * (pdx - buf.avgs[buf.index]))/(BUF_SIZE - 1));

  Serial.print("dx: ");
  Serial.println(dx);

  Serial.print("pdx: ");
  Serial.println(pdx);

  Serial.print("(dx - tmp) * (dx - buf.average): ");
  Serial.println((dx - tmp) * (dx - buf.average));

  Serial.print("(pdx - buf.rep_avg) * (pdx - buf.avgs[buf.index]): ");
  Serial.println((pdx - buf.rep_avg) * (pdx - buf.avgs[buf.index])); */

  if (count > BUF_SIZE)
    buf.variance += ((dx - tmp) * (dx - buf.average) - (pdx - buf.rep_avg) * (pdx - buf.avgs[buf.index]))/(BUF_SIZE - 1);
  else
    count++;

  buf.window[buf.index] = dx;
  buf.avgs[buf.index] = buf.average;
  buf.rep_avg = buf.window[buf.index];

  buf.index = (buf.index + 1) % BUF_SIZE;

  // Update the delay buffer
  dff.replaced = dff.window[dff.index];

  dff.window[dff.index] = buf.average;

  dff.index = (dff.index + 1) % DELAY_SIZE;

  // Updating the resting distance
  if (fabs(dff.replaced - buf.average) < EPSILON) {
    if (!alarmed) {
      alarmed = true;

      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
  
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
  
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    alarmed = false;
  }

  // Display data
  Serial.print(dx);
  Serial.print("\t");
  Serial.print(buf.average);
  Serial.print("\t");
  Serial.print(buf.variance);
  Serial.print("\t");
  Serial.print(dff.replaced);
  Serial.print("\t");
  Serial.print(dff.replaced - EPSILON);
  Serial.print("\t");
  Serial.println(dff.replaced + EPSILON);

  /*Serial.print("Post-Variance: ");
  Serial.print(buf.variance);

  Serial.println("\n=======================");

  delay(100);

  if (buf.variance > 10)
    exit(0); */
}
