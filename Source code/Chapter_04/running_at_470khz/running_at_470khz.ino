int pin = 2;  // this is pin header 2 
void setup() {
  // put your setup code here, to run once:
  
  pinMode(pin, OUTPUT_FAST);
}

void loop() {
    // put your main code here, to run repeatedly: 
    int state = LOW;
    while(1){
        digitalWrite(pin, state);
        state =!state;   // if as HIGH it will be LOW and vice-versa
    } 
}

