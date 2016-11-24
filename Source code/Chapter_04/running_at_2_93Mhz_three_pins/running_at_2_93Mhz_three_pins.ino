/*
   This program makes the I/O speed to achieve 2.93 MHz.
   If you are using Intel Galileo: Only pins 2 and 3 work
   If you are using Intel Galileo Gen 2: ALL pins works 
   except pins 7 and 8
   
   Note: if you are using Intel Galileo Gen 2 and change 
   this software to support pin 13, the frequency will be
   close to 1.16 Mhz.
*/

unsigned int latchValue;
unsigned int bmask_pin2;
unsigned int bmask_pin3;
unsigned int bmask_pin12;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(2, OUTPUT_FAST);
  pinMode(3, OUTPUT_FAST);
  pinMode(12, OUTPUT_FAST);
    
  // latches the current value     
  latchValue = fastGpioDigitalRegSnapshot(GPIO_FAST_IO2);

  // retrieving bitmasks from descriptors
  bmask_pin2 = 0x000000ff & GPIO_FAST_IO2;     //south-cluster
  bmask_pin3 = 0x000000ff & GPIO_FAST_IO3;     //south-cluster
  bmask_pin12 = 0x000000ff & GPIO_FAST_IO12;   //south-cluster
    
}

void loop() {
  
  while(1)
  {
        
     if (latchValue & bmask_pin12)
     {
        // state is HIGH
        latchValue = GPIO_FAST_IO2 & !bmask_pin2;
        latchValue |= GPIO_FAST_IO3 & !bmask_pin3;
        latchValue |= GPIO_FAST_IO12 & !bmask_pin12;

     }
     else
     {
        // state is LOW
        latchValue = GPIO_FAST_IO2 | bmask_pin2;
        latchValue |= GPIO_FAST_IO3 | bmask_pin3;
        latchValue |= GPIO_FAST_IO12 | bmask_pin12;

     }
    
     // considering all pins on this example belong the south-cluster
     // they share the same register in memory-mapped I/O. Only one 
     // fastGpioDigitalRegWriteUnsafe() must be called ensuring 2.93Mhz
     // to ALL pins
     fastGpioDigitalRegWriteUnsafe (GPIO_FAST_IO2, latchValue); 
 
  }

}
