/*
   This program is only a demonstration of fastGpioDigitalRegSnapshot()
   and the importance of the bitmask fields.
*/

void setup() {

  unsigned int latchValue;

  Serial.begin(115200);
  
  delay(3000); // only to give you time to open the serial debugger terminal
      
  // latches the current value     
  latchValue = fastGpioDigitalRegSnapshot(GPIO_FAST_IO3);

  // identifies the bit corresponts to pin 3 in the bitmask
  unsigned int mask = 0x000000ff & GPIO_FAST_IO3;
          
  if (latchValue & mask)
  {
       // the register indicated the pin is HIGH
       Serial.println("HIGH");
  }
  else
  {
       // the register indicated the pin is LOW
       Serial.println("LOW");
  }
 
}

void loop() {
  
}
