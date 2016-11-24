/*
   This program is an example how to mix pins from 
   north-cluster and south-cluster
   If you are using Intel Galileo: Only pins 2 and 3 work
   If you are using Intel Galileo Gen 2: ALL pins works 
   except pins 7 and 8

*/

unsigned int latchValue;
unsigned int bmask_pin2;
unsigned int bmask_pin3;
unsigned int bmask_pin4;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(2, OUTPUT_FAST);
  pinMode(3, OUTPUT_FAST);
  pinMode(4, OUTPUT_FAST);
    
  // latches the current value     
  latchValue = fastGpioDigitalRegSnapshot(GPIO_FAST_IO2);

  // retrieving bitmasks from descriptors
  bmask_pin2 = 0x000000ff & GPIO_FAST_IO2;     //south-cluster
  bmask_pin3 = 0x000000ff & GPIO_FAST_IO3;     //south-cluster
  bmask_pin4 = 0x000000ff & GPIO_FAST_IO4;     //north-cluster !!!!
    
}

void loop() {
  
  while(1)
  {
        
     if (latchValue & bmask_pin4)
     {
        // state is HIGH
        latchValue = GPIO_FAST_IO2 & !bmask_pin2;
        latchValue |= GPIO_FAST_IO3 & !bmask_pin3;
        latchValue |= GPIO_FAST_IO4 & !bmask_pin4;

     }
     else
     {
        // state is LOW
        latchValue = GPIO_FAST_IO2 | bmask_pin2;
        latchValue |= GPIO_FAST_IO3 | bmask_pin3;
        latchValue |= GPIO_FAST_IO4 | bmask_pin4;

     }
    
     // pins from cluster different used, so it is necessary
     // to make couple call using pins from south and north-cluster
     fastGpioDigitalRegWriteUnsafe (GPIO_FAST_IO2, latchValue); 
     fastGpioDigitalRegWriteUnsafe (GPIO_FAST_IO4, latchValue); 
 
  }

}
