/*
   This program makes the I/O speed to achieve 684 KHz.
   If you are using Intel Galileo: Only pins 2 and 3 work
   If you are using Intel Galileo Gen 2: ALL pins works 
   
   Note: if you are using Intel Galileo Gen 2 and change 
   this software to support pin 13, the frequency will be
   close to 657 KHz and not 687 KHz.
*/

unsigned int latchValue;
unsigned int bmask;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(2, OUTPUT_FAST);
  pinMode(3, OUTPUT_FAST);
    
  // latches the current value     
  latchValue = fastGpioDigitalRegSnapshot(GPIO_FAST_IO2);

  // identifies the bit corresponts to pin 3 in the bitmask
  bmask = 0x000000ff & GPIO_FAST_IO2;
    
}

void loop() {
  
  while(1)
  {
        
     if (latchValue & bmask)
     {
        // state is HIGH
        latchValue = GPIO_FAST_IO2 & !bmask;
     }
     else
     {
        // state is LOW
        latchValue = GPIO_FAST_IO2 | bmask;
     }
    
     fastGpioDigitalRegWriteUnsafe (GPIO_FAST_IO2, latchValue); 
 
  }

}
