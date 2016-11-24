/*
   This program is only a demonstration of fastGpioDigitalRead()
*/
int pin = 2;

#define MAX_COUNTER 200000

void setup() {

  Serial.begin(115200);
  
  pinMode(pin, INPUT_FAST);  // using the pin 2
  
  delay(3000); // only to give you time to open the serial debugger terminal
  
  Serial.print("Number of interactions under test:");
  Serial.println(MAX_COUNTER);
    
  unsigned long t0,t;
  unsigned int counter = 0;
  
  
  t0 = micros(); // number microseconds since booted 
  for (counter = 0; counter < MAX_COUNTER; counter++)
  {
     if (fastGpioDigitalRead(GPIO_FAST_IO2)) // using the fast I/O macro related
                                             // to pin 2
     {
         // the pin is HIGH
          Serial.println("HIGH detected by fastGpioDigitalRead()");
     }
  
  }
  
  t=micros()-t0; // delta time
  Serial.print("fastGpioDigitalRead() took: ");
  Serial.print(t);
  Serial.println(" microseconds");
  
  t0 = micros(); // reseting to new initial time
  for (counter = 0; counter < MAX_COUNTER; counter++)
  {
     if (digitalRead(pin)) // using the fast I/O macro related
                           // to pin 2
     {
         // the pin is HIGH
          Serial.println("HIGH  detected by digitalRead()");
     }
  
  }
 
  t=micros()-t0; // delta time
  Serial.print("digitalRead() took: ");
  Serial.print(t);
  Serial.println(" microseconds");
  
}

void loop() {
   

}
