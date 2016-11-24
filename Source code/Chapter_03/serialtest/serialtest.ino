
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);  // does not matter for this object
   delay(3000);
}

void loop() {
  // transmitting something from Galileo to IDE
  Serial.println("Hi there!! type something and press SEND button!!! ");
  delay(1000);

  // if the developer sent something from IDE to Galileo  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
  
    // add it to the inputString:
    inputString += inChar;
  
    if (inChar == '\n') {
      stringComplete = true;
      
    } 
  }
  
  if (stringComplete == true) {
    
    Serial.print("\nCOOL!! RECEIVED YOUR MESSAGE: ");
    Serial.println(inputString);
    inputString = "";
    stringComplete=false;
    
  }
  
}
