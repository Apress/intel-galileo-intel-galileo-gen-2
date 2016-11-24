/*
 * This program test the digital port speed 
 * using regular Arduino functions
 *
 * Intel Galileo: the max speed is 230Hz
 * Intel Galileo Gen 2: the speed is 470KHz to all pins except
 *                      pin 7 that achieves 1.8Khz  
 *
*/
int pin = 2;  // this is pin header 2 
void setup() {
  // put your setup code here, to run once:
  
  pinMode(pin, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly: 
    int state = LOW;
    while(1){
        digitalWrite(pin, state);
        state =!state;   // if as HIGH it will be LOW and vice-versa
    } 
}

