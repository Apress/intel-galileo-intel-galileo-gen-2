/*
   This program makes the I/O speed to achieve 684 KHz.
   If you are using Intel Galileo: Only pins 2 and 3 work
   If you are using Intel Galileo Gen 2: ALL pins works 
   
   Note: if you are using Intel Galileo Gen 2 and change 
   this software to support pin 13, the frequency will be
   close to 657 KHz and not 687 KHz.
*/

unsigned int latchValue, state, mask;

void setup() {
  // put your setup code here, to run once:
 
 
  Serial.begin(115200);
  delay(3000);
  pinMode(2, OUTPUT_FAST);
      
      
  // latches the current value     
  latchValue = fastGpioDigitalRegSnapshot(GPIO_FAST_IO2);

  // identifies the bit corresponts to pin 3 in the bitmask
  unsigned int mask = 0x000000ff & GPIO_FAST_IO2;
          
  // checking initial state of the pin        
  if (latchValue & mask)
  { 
      state = HIGH;
  }
  else
  {
      state = LOW;
  }
  
  Serial.println(mask);
  Serial.println(latchValue);
  Serial.println(state); 
}

void loop() {

  while(1)
  {
        
    if (state == LOW)
    {
        state = HIGH;
        latchValue |= mask;
    }
    else
    {
        state = LOW;
        //latchValue = 0;
    }

    fastGpioDigitalRegWriteUnsafe(GPIO_FAST_IO2, latchValue | mask);    
  }

}
