int pin = 2;
unsigned long duration;

void setup()
{
  Serial.begin(115200);
  pinMode(pin, INPUT);
}

void loop()
{
  // measuring the pulse lengh dmuring 20 seconds
  duration = pulseIn(pin, HIGH,20000000);
  Serial.println(duration);
}
