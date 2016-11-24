/*
   This program makes the I/O speed to achieve 684 KHz.
   If you are using Intel Galileo: Only pins 2 and 3 work
   If you are using Intel Galileo Gen 2: ALL pins works 
   
   Note: if you are using Intel Galileo Gen 2 and change 
   this software to support pin 13, the frequency will be
   close to 657 KHz and not 687 KHz.
*/
unsigned int pin = 2;  // this is pin header 3
void setup() {
  // put your setup code here, to run once:
  
  pinMode(pin, OUTPUT_FAST);
}

void loop() {
    // put your main code here, to run repeatedly: 
    int state = LOW;
    while(1){
        fastGpioDigitalWrite(GPIO_FAST_IO2, state);
        state =!state;   // if as HIGH it will be LOW and vice-versa
    }
  
}
