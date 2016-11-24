/*
   This program makes the I/O speed to achieve 1.12 MHz.
   If you are using Intel Galileo: Only pins 4 and 3 work
   If you are using Intel Galileo Gen 4: ALL pins works 
   except pins 7 and 8
*/

unsigned int latchValue;
unsigned int bmask;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(4, OUTPUT_FAST);
    
  // latches the current value     
  latchValue = fastGpioDigitalRegSnapshot(GPIO_FAST_IO4);

  // extract the mask that identifies the pin 4 in the
  // descriptor
  bmask = 0x000000ff & GPIO_FAST_IO4;  // NORTH-CLUSTER !!!!
    
}

void loop() {
  
  while(1)
  {
        
     if (latchValue & bmask)
     {
        // state is HIGH
        latchValue = GPIO_FAST_IO4 & !bmask;
     }
     else
     {
        // state is LOW
        latchValue = GPIO_FAST_IO4 | bmask;
     }
    
     fastGpioDigitalRegWriteUnsafe (GPIO_FAST_IO4, latchValue); 
 
  }

}
