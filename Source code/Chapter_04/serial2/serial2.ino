// Testing the Serial 2


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {
  // put your setup code here, to run once:
   Serial2.begin(115200);
   Serial.begin(115200);
}

void loop() {
  
  
  Serial2.println("Intel"); // this will transmitt using PIN 2
  inputString="";   
  
  // if the developer sent something from IDE to Galileo  
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();  // receiving from PIN 3
  
    // add it to the inputString:
    inputString += inChar;
  
    if (inChar == '\n') {
      stringComplete = true;
      
    } 
  }
 
  if (stringComplete == true) {
    
    Serial.print("Message received:");
    Serial.println(inputString);
    stringComplete=false;
    
  }
 
  delay(5000);
  
}
